/*****************************************************************************
Copyright (c) 2020, 2025, Oracle and/or its affiliates.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2.0, as published by the
Free Software Foundation.

This program is designed to work with certain software (including
but not limited to OpenSSL) that is licensed under separate terms,
as designated in a particular file or component or in included license
documentation.  The authors of MySQL hereby grant you an additional
permission to link the program and your derivative works with the
separately licensed software that they have either included with
the program or referenced in the documentation.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License, version 2.0,
for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA

*****************************************************************************/

/** @file dict/dict0inst.cc
Instant DDL 接口实现。

本文件实现了 Instant DDL 的核心逻辑，包括：
1. commit_instant_ddl() —— 根据 Instant_Type 分发到不同的提交路径
2. is_instant_add_drop_possible() —— 行大小校验，确保 Instant ADD 后不会超限
3. commit_instant_add_col_low() / commit_instant_drop_col_low() —— DD 元数据更新
4. populate_to_be_instant_columns_low() —— 对比新旧表列定义，识别 ADD/DROP 列

Created 2020-04-24 by Mayank Prasad */

#include "dict0inst.h"
#include "dict0dd.h"
#include "dict0dict.h"
#include "ha_innodb.h"

static void populate_to_be_instant_columns_low(
    const Alter_inplace_info *ha_alter_info, const TABLE *old_table,
    const TABLE *new_table, Columns &cols_to_add, Columns &cols_to_drop);

/**
 检查 Instant ADD/DROP 是否可行。
 核心逻辑：计算当前表行的最大可能大小，再逐个累加新增列的最大大小，
 如果超过页面允许的最大记录大小（page_rec_max），则拒绝 Instant。
 这是因为 Instant ADD 不会重写数据，如果新行可能超大，
 后续 INSERT 会失败，所以必须在 DDL 阶段就拒绝。
*/
template <typename Table>
bool Instant_ddl_impl<Table>::is_instant_add_drop_possible(
    const Alter_inplace_info *ha_alter_info, const TABLE *table,
    const TABLE *altered_table, const dict_table_t *dict_table) {
  Columns cols_to_add;
  Columns cols_to_drop;
  populate_to_be_instant_columns_low(ha_alter_info, table, altered_table,
                                     cols_to_add, cols_to_drop);

  if (cols_to_add.empty() && cols_to_drop.empty()) {
    ut_ad(0);
    return true;
  }

  const dict_index_t *index = dict_table->first_index();

  /* Get maximum permissible size on page */
  size_t page_rec_max;
  size_t page_ptr_max;
  get_permissible_max_size(dict_table, index, page_rec_max, page_ptr_max);

  /* Get the maximum size of a valid rec in current table */
  size_t current_max_size;
  const bool res = dict_index_validate_max_rec_size(
      dict_table, index, page_rec_max, page_ptr_max, current_max_size);

  if (res) {
    /* Table is already in a state where possible row size can go beyond
    permissible size limit. Don't allow INSTANT ADD */
    ib::error(ER_IB_INSTANT_ADD_DROP_COLUMN_NOT_ALLOWED,
              dict_table->name.m_name, current_max_size, page_rec_max);
    return false;
  }

  for (auto field : cols_to_add) {
    if (innobase_is_v_fld(field)) {
      continue;
    }

    /* Get the maximum possible size needed for this field */
    size_t field_max_size = 0;
    {
      unsigned col_len;
      ulint mtype;
      ulint prtype;
      get_field_types(nullptr, dict_table, field, col_len, mtype, prtype);

      /* Create a dummy dict_cot_t and dict_field_t just to calculate size */
      dict_col_t dummy_col;
      dummy_col.mtype = (unsigned int)mtype;
      dummy_col.prtype = (unsigned int)prtype;
      dummy_col.len = (unsigned int)col_len;
      ulint mbminlen;
      ulint mbmaxlen;
      dtype_get_mblen(mtype, prtype, &mbminlen, &mbmaxlen);
      dummy_col.set_mbminmaxlen(mbminlen, mbmaxlen);
      dict_field_t dummy_field;
      dummy_field.col = &dummy_col;
      get_field_max_size(dict_table, index, &dummy_field, field_max_size);
    }

    current_max_size += field_max_size;
    if (current_max_size > page_rec_max) {
      /* Don't allow INSTANT ADD */
      ib::error(ER_IB_INSTANT_ADD_NOT_POSSIBLE, cols_to_add.size(),
                dict_table->name.m_name, current_max_size, page_rec_max);
      return false;
    }
  }

  return true;
}

template bool Instant_ddl_impl<dd::Table>::is_instant_add_drop_possible(
    const Alter_inplace_info *ha_alter_info, const TABLE *table,
    const TABLE *altered_table, const dict_table_t *dict_table);

template bool Instant_ddl_impl<dd::Partition>::is_instant_add_drop_possible(
    const Alter_inplace_info *ha_alter_info, const TABLE *table,
    const TABLE *altered_table, const dict_table_t *dict_table);

/**
 Instant ADD COLUMN 底层实现。
 步骤：
 1. 调用 dd_copy_table_columns() 将旧列的默认值信息复制到新 DD 表
 2. 调用 dd_add_instant_columns() 为新增列在 DD 中记录默认值
    （这些默认值存储在列的 se_private_data 中，读取旧行时用于填充新列）
 3. 调用 dd_update_v_cols() 更新虚拟列元数据
*/
template <typename Table>
bool Instant_ddl_impl<Table>::commit_instant_add_col_low() {
  ut_ad(!m_dict_table->is_temporary());

  ut_a(is_instant_add_drop_possible(m_ha_alter_info, m_old_table,
                                    m_altered_table, m_dict_table));

  /* To remember old default values if exist */
  dd_copy_table_columns(m_ha_alter_info, m_new_dd_tab->table(),
                        m_old_dd_tab->table(), m_dict_table);

  /* Then add all new default values */
  if (dd_add_instant_columns(&m_old_dd_tab->table(), &m_new_dd_tab->table(),
                             m_dict_table, m_cols_to_add))
    return true;

  /* Keep the metadata for newly added virtual columns if exist */
  dd_update_v_cols(&m_new_dd_tab->table(), m_dict_table->id);

  return false;
}

template bool Instant_ddl_impl<dd::Table>::commit_instant_add_col_low();
template bool Instant_ddl_impl<dd::Partition>::commit_instant_add_col_low();

template <>
bool Instant_ddl_impl<dd::Table>::commit_instant_add_col() {
  return commit_instant_add_col_low();
}

template <>
bool Instant_ddl_impl<dd::Partition>::commit_instant_add_col() {
  if (dd_part_is_first(m_new_dd_tab)) {
    if (commit_instant_add_col_low()) return true;
  }
  return false;
}

/**
 Instant DROP COLUMN 底层实现。
 步骤：
 1. 调用 dd_copy_table_columns() 复制列元数据
 2. 调用 dd_drop_instant_columns() 将被删除列标记为 "dropped"
    在 DD 中记录其物理位置（column position、version_added/dropped 等），
    使得旧行中该列的数据在读取时被正确跳过。
    物理数据并不会立即删除，而是在后续表重建时清理。
*/
template <typename Table>
bool Instant_ddl_impl<Table>::commit_instant_drop_col_low() {
  ut_ad(!m_dict_table->is_temporary());

  ut_a(is_instant_add_drop_possible(m_ha_alter_info, m_old_table,
                                    m_altered_table, m_dict_table));

  /* Copy columns metadata */
  dd_copy_table_columns(m_ha_alter_info, m_new_dd_tab->table(),
                        m_old_dd_tab->table(), m_dict_table);

  /* Update metadata of columns to be dropped */
  return dd_drop_instant_columns(&m_old_dd_tab->table(), &m_new_dd_tab->table(),
                                 m_dict_table, m_cols_to_drop
#ifdef UNIV_DEBUG
                                 ,
                                 m_cols_to_add, m_ha_alter_info
#endif
  );
}

template bool Instant_ddl_impl<dd::Table>::commit_instant_drop_col_low();
template bool Instant_ddl_impl<dd::Partition>::commit_instant_drop_col_low();

template <>
bool Instant_ddl_impl<dd::Table>::commit_instant_drop_col() {
  return commit_instant_drop_col_low();
}

template <>
bool Instant_ddl_impl<dd::Partition>::commit_instant_drop_col() {
  if (dd_part_is_first(m_new_dd_tab)) {
    if (commit_instant_drop_col_low()) return true;
  }
  return false;
}

/**
 commit_instant_ddl() —— Instant DDL 的提交入口。
 根据 handler_trivial_ctx 中存储的 Instant_Type 分发到不同路径：

 INSTANT_NO_CHANGE:
   仅复制 DD 私有数据，不做任何实际变更。

 INSTANT_COLUMN_RENAME:
   更新列名 + 刷新字典缓存（innobase_discard_table 使旧缓存失效）。

 INSTANT_VIRTUAL_ONLY:
   更新虚拟列元数据 + 处理 FTS_DOC_ID 隐藏列 + 刷新字典缓存。

 INSTANT_ADD_DROP_COLUMN（核心路径）:
   1. 开启事务
   2. populate_to_be_instant_columns() 识别新增/删除的列
   3. commit_instant_drop_col() 处理删除列
   4. commit_instant_add_col() 处理新增列
   5. 递增 current_row_version（行版本号，用于区分新旧格式行）
   6. 记录事务 ID 到索引元数据
   7. 刷新字典缓存
*/
template <typename Table>
bool Instant_ddl_impl<Table>::commit_instant_ddl() {
  Instant_Type type =
      static_cast<Instant_Type>(m_ha_alter_info->handler_trivial_ctx);

  switch (type) {
    case Instant_Type::INSTANT_NO_CHANGE:
      dd_commit_inplace_no_change(false);
      break;
    case Instant_Type::INSTANT_COLUMN_RENAME:
      dd_commit_inplace_no_change(false);

      if (!dd_table_is_partitioned(m_new_dd_tab->table()) ||
          dd_part_is_first(reinterpret_cast<dd::Partition *>(m_new_dd_tab))) {
        dd_update_v_cols(&m_new_dd_tab->table(), m_dict_table->id);
      }

      row_mysql_lock_data_dictionary(m_trx, UT_LOCATION_HERE);
      innobase_discard_table(m_thd, m_dict_table);
      row_mysql_unlock_data_dictionary(m_trx);
      break;
    case Instant_Type::INSTANT_VIRTUAL_ONLY:
      if (dd_find_column(&m_old_dd_tab->table(), FTS_DOC_ID_COL_NAME) &&
          !dd_find_column(&m_new_dd_tab->table(), FTS_DOC_ID_COL_NAME)) {
        dd::Column *col = dd_add_hidden_column(
            &m_new_dd_tab->table(), FTS_DOC_ID_COL_NAME, FTS_DOC_ID_LEN,
            dd::enum_column_types::LONGLONG);
        dd_set_hidden_unique_index(m_new_dd_tab->table().add_index(),
                                   FTS_DOC_ID_INDEX_NAME, col);
      }
      dd_commit_inplace_no_change(true);

      if (!dd_table_is_partitioned(m_new_dd_tab->table()) ||
          dd_part_is_first(reinterpret_cast<dd::Partition *>(m_new_dd_tab))) {
        dd_update_v_cols(&m_new_dd_tab->table(), m_dict_table->id);
      }

      row_mysql_lock_data_dictionary(m_trx, UT_LOCATION_HERE);
      innobase_discard_table(m_thd, m_dict_table);
      row_mysql_unlock_data_dictionary(m_trx);
      break;
    case Instant_Type::INSTANT_ADD_DROP_COLUMN:
      trx_start_if_not_started(m_trx, true, UT_LOCATION_HERE);
      dd_copy_private(*m_new_dd_tab, *m_old_dd_tab);

      /* Fetch the columns which are to be added or dropped */
      populate_to_be_instant_columns();

      ut_ad(!m_cols_to_add.empty() || !m_cols_to_drop.empty());

      if (!m_cols_to_drop.empty()) {
        /* INSTANT DROP */
        if (commit_instant_drop_col()) return true;
      }

      if (!m_cols_to_add.empty()) {
        /* INSTANT ADD */
        if (commit_instant_add_col()) return true;
      }

      /* Update the current row version in dictionary cache */
      m_dict_table->current_row_version++;

      ut_ad(dd_table_has_instant_cols(m_new_dd_tab->table()));

      for (auto dd_index : *m_new_dd_tab->indexes()) {
        dd::Properties &p = dd_index->se_private_data();
        p.set(dd_index_key_strings[DD_INDEX_TRX_ID], m_trx->id);
      }

      row_mysql_lock_data_dictionary(m_trx, UT_LOCATION_HERE);
      innobase_discard_table(m_thd, m_dict_table);
      row_mysql_unlock_data_dictionary(m_trx);

      break;
    case Instant_Type::INSTANT_IMPOSSIBLE:
    default:
      ut_ad(0);
  }

  if (m_autoinc != nullptr) {
    ut_ad(m_altered_table->found_next_number_field != nullptr);
    if (!dd_table_is_partitioned(m_new_dd_tab->table()) ||
        dd_part_is_first(reinterpret_cast<dd::Partition *>(m_new_dd_tab))) {
      dd_set_autoinc(m_new_dd_tab->table().se_private_data(), *m_autoinc);
    }
  }
  return false;
}

template bool Instant_ddl_impl<dd::Table>::commit_instant_ddl();
template bool Instant_ddl_impl<dd::Partition>::commit_instant_ddl();

/**
 识别需要 Instant ADD 和 DROP 的列（底层实现）。

 算法：
 1. 先收集所有被重命名的列（重命名不算 ADD/DROP）
 2. 遍历旧表的每一列，在新表中查找同名列：
    - 找不到 → 该列被 DROP
    - 找到但在 drop list 中 → 该列被 DROP，同时检查新表中同名列是否是新增的
 3. 遍历新表的每一列，在旧表中查找同名列：
    - 找不到 → 该列被 ADD
    - 找到但旧列被重命名 → 该列是新增的（旧列名已被重命名走了）

 注意：虚拟列被跳过，因为虚拟列不占物理存储。
*/
static void populate_to_be_instant_columns_low(
  /* Collect all renamed columns */
  using renamed_fields_t = std::pair<std::string, std::string>;
  std::vector<renamed_fields_t> renamed_fields;
  for (size_t i = 0; i < old_table->s->fields; i++) {
    const char *field_name = old_table->field[i]->field_name;
    std::string new_name;
    if (is_renamed(ha_alter_info, field_name, new_name)) {
      ut_a(!new_name.empty());
      renamed_fields.push_back(std::pair(field_name, new_name));
    }
  }

  for (size_t i = 0; i < old_table->s->fields; i++) {
    Field *old_table_field = old_table->field[i];
    const char *old_field_name = old_table_field->field_name;

    /* Skip virtual column from old table */
    if (innobase_is_v_fld(old_table_field)) {
      continue;
    }

    /* Skip if this column is being renamed */
    auto it = std::find_if(
        renamed_fields.begin(), renamed_fields.end(),
        [&old_field_name](renamed_fields_t &element) {
          return (strcmp(element.first.c_str(), old_field_name) == 0);
        });
    if (it != renamed_fields.end()) {
      continue;
    }

    /* Look for this column in new table */
    bool found = false;
    for (size_t j = 0; j < altered_table->s->fields; j++) {
      Field *new_table_field = altered_table->field[j];

      /* Skip virtual column from altered table */
      if (innobase_is_v_fld(new_table_field)) {
        continue;
      }

      const char *new_field_name = new_table_field->field_name;

      if (strcmp(old_field_name, new_field_name) == 0) {
        /* This column is present in both the tables. Stop iteration. */

        /* Check if this column is in drop list of alter_info. */
        if (is_dropped(ha_alter_info, old_field_name)) {
          /* This column is being dropped */
          cols_to_drop.push_back(old_table_field);

          /* But this column is present in new table as well which is possible
          if a new column with same name is being added or an existing column
          is being renamed to this name. */
          auto it = std::find_if(
              renamed_fields.begin(), renamed_fields.end(),
              [&new_field_name](renamed_fields_t &element) {
                return (strcmp(element.second.c_str(), new_field_name) == 0);
              });
          if (it == renamed_fields.end()) {
            /* Not renamed, so must be being added */
            cols_to_add.push_back(new_table_field);
          }
        }

        found = true;
        break;
      }
    }

    /* Could not find this column in new table. So it is being dropped. */
    if (!found) {
      cols_to_drop.push_back(old_table_field);
    }
  } /* For */

  for (size_t i = 0; i < altered_table->s->fields; i++) {
    Field *new_table_field = altered_table->field[i];

    /* Skip virtual column from altered table */
    if (innobase_is_v_fld(new_table_field)) {
      continue;
    }

    const char *new_field_name = new_table_field->field_name;
    /* Skip if it is renamed field */
    auto it = std::find_if(
        renamed_fields.begin(), renamed_fields.end(),
        [&new_field_name](renamed_fields_t &element) {
          return (strcmp(element.second.c_str(), new_field_name) == 0);
        });
    if (it != renamed_fields.end()) {
      continue;
    }

    /* Look for this column in old table */
    bool found = false;
    for (size_t j = 0; j < old_table->s->fields; j++) {
      Field *old_table_field = old_table->field[j];

      /* Skip virtual column from old table */
      if (innobase_is_v_fld(old_table_field)) {
        continue;
      }

      const char *old_field_name = old_table_field->field_name;

      /* This column is present in both the tables. Stop iteration. */
      if (strcmp(old_field_name, new_field_name) == 0) {
        /* If the old column is renamed, then this new column is being added */
        auto it = std::find_if(
            renamed_fields.begin(), renamed_fields.end(),
            [&old_field_name](renamed_fields_t &element) {
              return (strcmp(element.first.c_str(), old_field_name) == 0);
            });
        if (it != renamed_fields.end()) {
          cols_to_add.push_back(new_table_field);
        }

        found = true;
        break;
      }
    }

    /* Could not find this column in old table. So it is being added. */
    if (!found) {
      cols_to_add.push_back(new_table_field);
    }
  } /* FOR */

  renamed_fields.clear();
}

template <typename Table>
void Instant_ddl_impl<Table>::populate_to_be_instant_columns() {
  populate_to_be_instant_columns_low(m_ha_alter_info, m_old_table,
                                     m_altered_table, m_cols_to_add,
                                     m_cols_to_drop);
}

template <typename Table>
void Instant_ddl_impl<Table>::dd_commit_inplace_no_change(bool ignore_fts) {
  if (dd_table_has_instant_drop_cols(m_old_dd_tab->table())) {
    /* Copy dropped columns from old table to new table */
    copy_dropped_columns(&m_old_dd_tab->table(), &m_new_dd_tab->table(),
                         UINT32_UNDEFINED);
  }

  if (!ignore_fts) {
    dd_add_fts_doc_id_index(m_new_dd_tab->table(), m_old_dd_tab->table());
  }

  dd_copy_private(*m_new_dd_tab, *m_old_dd_tab);

  if (!dd_table_is_partitioned(m_new_dd_tab->table()) ||
      dd_part_is_first(reinterpret_cast<dd::Partition *>(m_new_dd_tab))) {
    dd_copy_table(m_ha_alter_info, m_new_dd_tab->table(),
                  m_old_dd_tab->table());
  }
}

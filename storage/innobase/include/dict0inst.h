/*****************************************************************************
Copyright (c) 2018, 2025, Oracle and/or its affiliates.

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

/** @file dict0inst.h
Instant DDL 接口头文件。

Instant DDL 是 MySQL 8.0 引入的一种极快的 DDL 算法，仅修改数据字典元数据，
不需要触碰任何表数据页。典型操作包括：
- 在表末尾添加列（带默认值）
- 删除列（标记为 dropped，物理数据延迟清理）
- 重命名列
- 添加/删除虚拟列

核心思想：通过在数据字典中记录"行版本号"(row version) 和列的默认值，
使得旧格式的行在读取时能够被正确解释，从而避免重写数据。

Created 2020-04-24 by Mayank Prasad. */

#ifndef dict0inst_h
#define dict0inst_h

#include "dict0dd.h"
#include "ha_innopart.h"
#include "handler0alter.h"

/**
 Instant DDL 操作类型枚举。
 在 check_if_supported_inplace_alter() 中确定，存储在
 ha_alter_info->handler_trivial_ctx 中，commit 阶段根据此值
 选择不同的元数据更新路径。
*/
enum class Instant_Type : uint16_t {
  /** 不可能使用 Instant 算法（需要 INPLACE 或 COPY） */
  INSTANT_IMPOSSIBLE,

  /** 无需任何实际变更（如仅改注释等），但仍走 Instant 路径更新 DD 元数据 */
  INSTANT_NO_CHANGE,

  /** 仅添加或删除虚拟列（虚拟列不占物理存储，只需更新元数据） */
  INSTANT_VIRTUAL_ONLY,

  /** 可以 Instant 执行的 ADD/DROP COLUMN 操作。
  包括添加/删除存储列（可同时伴随虚拟列变更）。
  这是 Instant DDL 最核心的场景：通过行版本号机制，
  新增列的默认值记录在 DD 中，旧行读取时自动填充默认值；
  删除列标记为 dropped，旧行读取时跳过该列。 */
  INSTANT_ADD_DROP_COLUMN,

  /** 仅列重命名（只需更新数据字典中的列名，不涉及数据变更） */
  INSTANT_COLUMN_RENAME
};

using Columns = std::vector<Field *>;

/**
 Instant DDL 的核心实现类（模板类，支持普通表和分区表）。

 模板参数 Table 为 dd::Table 或 dd::Partition，分别对应普通表和分区表。
 分区表的特殊之处在于：只有第一个分区需要更新表级元数据（列定义等），
 其余分区只需更新分区级私有数据。

 主要职责：
 1. commit_instant_ddl() —— 根据 Instant_Type 分发到不同的提交路径
 2. is_instant_add_drop_possible() —— 检查行大小是否允许 Instant ADD/DROP
 3. commit_instant_add_col_low() / commit_instant_drop_col_low() —— 执行元数据变更

 @see Instant_Type 决定了 commit_instant_ddl() 的分发逻辑
*/
template <typename Table>
class Instant_ddl_impl {
 public:
  Instant_ddl_impl() = delete;

  /** Constructor
  @param[in]        alter_info    inplace alter information
  @param[in]        thd           user thread
  @param[in]        trx           transaction
  @param[in,out]    dict_table    innodb dictionary cache object
  @param[in]        old_table     old global DD cache object
  @param[in,out]    altered_table new MySQL table object
  @param[in]        old_dd_tab    old global DD cache object
  @param[in,out]    new_dd_tab    new global DD cache object
  @param[in]        autoinc       auto increment */
  Instant_ddl_impl(Alter_inplace_info *alter_info, THD *thd, trx_t *trx,
                   dict_table_t *dict_table, const TABLE *old_table,
                   const TABLE *altered_table, const Table *old_dd_tab,
                   Table *new_dd_tab, uint64_t *autoinc)
      : m_ha_alter_info(alter_info),
        m_thd(thd),
        m_trx(trx),
        m_dict_table(dict_table),
        m_old_table(old_table),
        m_altered_table(altered_table),
        m_old_dd_tab(old_dd_tab),
        m_new_dd_tab(new_dd_tab),
        m_autoinc(autoinc) {}

  /** Destructor */
  ~Instant_ddl_impl() {}

  /** 提交 Instant DDL 变更。
  根据 handler_trivial_ctx 中存储的 Instant_Type，分发到不同的处理路径：
  - INSTANT_NO_CHANGE: 仅复制 DD 私有数据
  - INSTANT_COLUMN_RENAME: 更新列名 + 刷新字典缓存
  - INSTANT_VIRTUAL_ONLY: 更新虚拟列元数据 + 刷新字典缓存
  - INSTANT_ADD_DROP_COLUMN: 核心路径，更新行版本号、列默认值等
  @retval true Failure
  @retval false Success */
  bool commit_instant_ddl();

  /** 检查 INSTANT ADD/DROP 是否可行。
  主要验证：添加新列后，行的最大可能大小是否会超过页面允许的最大记录大小。
  InnoDB 页面对单行大小有限制（约为页大小的一半），如果 Instant ADD 后
  可能产生超大行，则拒绝 Instant 算法，回退到 INPLACE（需要重建表）。
  @param[in]  ha_alter_info alter info
  @param[in]  table         MySQL table before ALTER
  @param[in]  altered_table MySQL table after ALTER
  @param[in]  dict_table    InnoDB table definition cache
  @return true if INSTANT ADD/DROP can be done, false otherwise. */
  static bool is_instant_add_drop_possible(
      const Alter_inplace_info *ha_alter_info, const TABLE *table,
      const TABLE *altered_table, const dict_table_t *dict_table);

 private:
  /** Instant ADD COLUMN 的底层实现。
  将旧列的默认值复制到新 DD 表定义中，然后为新增列添加 Instant 默认值。
  这些默认值会被存储在 DD 的列 se_private_data 中，当读取旧版本行时，
  InnoDB 会用这些默认值填充新增列。
  @retval true Failure
  @retval false Success */
  bool commit_instant_add_col_low();

  /** Instant DROP COLUMN 的底层实现。
  将被删除列标记为 "dropped"，在 DD 元数据中记录其物理位置信息，
  使得旧行中该列的数据在读取时被跳过。物理数据不会立即删除，
  而是在后续的表重建（OPTIMIZE TABLE 等）时才真正清理。
  @retval true Failure
  @retval false Success */
  bool commit_instant_drop_col_low();

  /** Instant ADD COLUMN 入口（处理分区表逻辑）。
  对于分区表，只有第一个分区需要执行 commit_instant_add_col_low()。
  @retval true Failure
  @retval false Success */
  bool commit_instant_add_col();

  /** Instant DROP COLUMN 入口（处理分区表逻辑）。
  对于分区表，只有第一个分区需要执行 commit_instant_drop_col_low()。
  @retval true Failure
  @retval false Success */
  bool commit_instant_drop_col();

  /** 收集需要 Instant ADD 和 DROP 的列列表。
  通过对比新旧表的列定义，识别出哪些列是新增的、哪些是被删除的，
  同时正确处理列重命名的情况（重命名的列不算 ADD/DROP）。 */
  void populate_to_be_instant_columns();

  /** 当 ALTER TABLE 不涉及实际数据变更时，更新 DD 元数据。
  复制旧表的 DD 私有数据到新表，处理 dropped 列信息和 FTS 索引。
  @param[in]      ignore_fts      ignore FTS update if true */
  void dd_commit_inplace_no_change(bool ignore_fts);

 private:
  /* 需要 Instant 添加的列列表（由 populate_to_be_instant_columns 填充） */
  Columns m_cols_to_add;

  /* 需要 Instant 删除的列列表（由 populate_to_be_instant_columns 填充） */
  Columns m_cols_to_drop;

  /* Inplace ALTER 的上下文信息，包含变更标志、handler_trivial_ctx 等 */
  Alter_inplace_info *m_ha_alter_info;

  /* 用户线程 */
  THD *m_thd;

  /* InnoDB 事务对象，用于 Instant DDL 的元数据更新操作 */
  trx_t *m_trx;

  /* InnoDB 字典缓存中的表对象，Instant DDL 会直接修改其 current_row_version */
  dict_table_t *m_dict_table;

  /* ALTER 前的 MySQL 表对象 */
  const TABLE *m_old_table;

  /* ALTER 后的 MySQL 表对象（新的表定义） */
  const TABLE *m_altered_table;

  /* 旧的 DD 表/分区定义（用于复制元数据） */
  const Table *m_old_dd_tab;

  /* 新的 DD 表/分区定义（Instant DDL 的目标，元数据写入此对象） */
  Table *m_new_dd_tab;

  /* AUTO_INCREMENT 值指针，Instant DDL 完成后需要更新 */
  uint64_t *m_autoinc;
};

#endif /* dict0inst_h */

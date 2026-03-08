/*****************************************************************************

Copyright (c) 2011, 2025, Oracle and/or its affiliates.

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

#include "my_psi_config.h"

/** @file include/row0log.h
 Online DDL 的行日志（Row Log）模块。

 Row Log 是 Online DDL 能够在不阻塞并发 DML 的情况下工作的核心机制。
 当 DDL 在后台执行数据变更（如重建表或创建索引）时，并发的 INSERT/UPDATE/DELETE
 操作会被记录到 row log 中。DDL 完成数据扫描后，在 commit 阶段短暂持有排他锁，
 将 row log 中积累的变更回放（apply）到新表或新索引上。

 Row Log 分为两种：
 1. 表级 row log（用于 Online 表重建）：记录到聚簇索引的 online_log 中
    - row_log_table_insert() / row_log_table_update() / row_log_table_delete()
    - 通过 row_log_table_apply() 回放
 2. 索引级 row log（用于 Online 创建二级索引）：记录到新索引的 online_log 中
    - row_log_online_op()
    - 通过 row_log_apply() 回放

 Row Log 数据写入临时文件，大小受 innodb_online_alter_log_max_size 限制。
 如果并发 DML 产生的日志超过此限制，DDL 会失败并报 DB_ONLINE_LOG_TOO_BIG 错误。

 Created 2011-05-26 Marko Makela
 *******************************************************/

#ifndef row0log_h
#define row0log_h

#include "univ.i"

#include "data0types.h"
#include "dict0types.h"
#include "mtr0types.h"
#include "que0types.h"
#include "rem0types.h"
#include "row0types.h"
#include "trx0types.h"

class Alter_stage;

/** 为索引分配 row log，并将索引标记为"正在在线创建"。
这是 Online DDL 的关键初始化步骤：
- 对于二级索引创建：table=NULL，row log 记录并发 DML 对该索引的影响
- 对于表重建：table=新表，row log 记录到旧表聚簇索引上，
  记录并发 DML 的行变更，commit 阶段回放到新表
@param[in] index    Index.
@param[in] table    New table being rebuilt, or NULL when creating a secondary
index.
@param[in] same_pk  Whether the definition of the PRIMARY KEY has remained the
same.
@param[in] add_cols Default values of added columns, or nullptr.
@param[in] col_map  Mapping of old column numbers to new ones, or nullptr if
!table.
@param[in] path     Where to create temporary file.
@retval true if success, false if not */
bool row_log_allocate(dict_index_t *index, dict_table_t *table, bool same_pk,
                      const dtuple_t *add_cols, const ulint *col_map,
                      const char *path);

/** 释放索引的 row log 内存。 */
void row_log_free(row_log_t *&log); /*!< in,own: row log */

#ifndef UNIV_HOTBACKUP
/** 释放在线创建被中止的索引的 row log。 */
static inline void row_log_abort_sec(
    dict_index_t *index); /*!< in/out: index (x-latched) */

/** 尝试将一个操作记录到正在（或曾经）在线创建的二级索引的 row log 中。
当并发 DML 修改了与正在创建的二级索引相关的数据时调用。
@param[in,out] index Index, S- or X-latched.
@param[in] tuple Index tuple.
@param[in] trx_id Transaction ID for insert, or 0 for delete.
@retval true if the operation was logged or can be ignored
@retval false if online index creation is not taking place */
[[nodiscard]] static inline bool row_log_online_op_try(dict_index_t *index,
                                                       const dtuple_t *tuple,
                                                       trx_id_t trx_id);
#endif /* !UNIV_HOTBACKUP */
/** 将操作记录到正在（或曾经）在线创建的二级索引的 row log 中。
与 row_log_online_op_try 不同，此函数不检查索引是否正在创建，直接写入日志。
用于二级索引的在线创建：当 DDL 扫描聚簇索引构建新索引的同时，
并发 DML 通过此函数将变更记录到 row log，DDL 完成后回放。 */
void row_log_online_op(
    dict_index_t *index,   /*!< in/out: index, S or X latched */
    const dtuple_t *tuple, /*!< in: index tuple */
    trx_id_t trx_id)       /*!< in: transaction ID for insert,
                           or 0 for delete */
    UNIV_COLD;

/** 获取在线索引重建 row log 的错误状态。
如果并发 DML 产生的日志超过 innodb_online_alter_log_max_size，
或者发生了其他错误，此函数返回相应的错误码。
 @return DB_SUCCESS or error code */
[[nodiscard]] dberr_t row_log_table_get_error(
    const dict_index_t *index); /*!< in: clustered index of a table
                               that is being rebuilt online */

/** Check whether a virtual column is indexed in the new table being
created during alter table
@param[in]      index   cluster index
@param[in]      v_no    virtual column number
@return true if it is indexed, else false */
bool row_log_col_is_indexed(const dict_index_t *index, ulint v_no);

/** 记录一个 DELETE 操作到正在重建的表的 row log 中。
当 Online 表重建期间有并发 DELETE 时调用，
日志会在 commit 阶段通过 row_log_table_apply() 中的
row_log_table_apply_delete() 回放到新表。 */
void row_log_table_delete(
    const rec_t *rec,       /*!< in: clustered index leaf page record,
                            page X-latched */
    const dtuple_t *ventry, /*!< in: dtuple holding virtual column info */
    dict_index_t *index,    /*!< in/out: clustered index, S-latched
                            or X-latched */
    const ulint *offsets,   /*!< in: rec_get_offsets(rec,index) */
    const byte *sys)        /*!< in: DB_TRX_ID,DB_ROLL_PTR that should
                            be logged, or NULL to use those in rec */
    UNIV_COLD;

/** 记录一个 UPDATE 操作到正在重建的表的 row log 中。
当 Online 表重建期间有并发 UPDATE 时调用，
日志会在 commit 阶段通过 row_log_table_apply() 中的
row_log_table_apply_update() 回放到新表。 */
void row_log_table_update(
    const rec_t *rec,          /*!< in: clustered index leaf page record,
                               page X-latched */
    dict_index_t *index,       /*!< in/out: clustered index, S-latched
                               or X-latched */
    const ulint *offsets,      /*!< in: rec_get_offsets(rec,index) */
    const dtuple_t *old_pk,    /*!< in: row_log_table_get_pk()
                               before the update */
    const dtuple_t *new_v_row, /*!< in: dtuple contains the new virtual
                             columns */
    const dtuple_t *old_v_row) /*!< in: dtuple contains the old virtual
                             columns */
    UNIV_COLD;

/** 从旧表的聚簇索引记录中构造旧的 PRIMARY KEY 和 DB_TRX_ID, DB_ROLL_PTR。
用于 Online 表重建时，在记录 UPDATE/DELETE 到 row log 之前，
需要先获取旧行的主键值，以便在回放时能正确定位新表中的对应行。
 @return tuple of PRIMARY KEY,DB_TRX_ID,DB_ROLL_PTR in the rebuilt table,
 or NULL if the PRIMARY KEY definition does not change */
[[nodiscard]] const dtuple_t *row_log_table_get_pk(
    const rec_t *rec,     /*!< in: clustered index leaf page record,
                          page X-latched */
    dict_index_t *index,  /*!< in/out: clustered index, S-latched
                          or X-latched */
    const ulint *offsets, /*!< in: rec_get_offsets(rec,index),
                          or NULL */
    byte *sys,            /*!< out: DB_TRX_ID,DB_ROLL_PTR for
                          row_log_table_delete(), or NULL */
    mem_heap_t **heap)    /*!< in/out: memory heap where allocated */
    UNIV_COLD;

/** 记录一个 INSERT 操作到正在重建的表的 row log 中。
当 Online 表重建期间有并发 INSERT 时调用，
日志会在 commit 阶段通过 row_log_table_apply() 中的
row_log_table_apply_insert() 回放到新表。 */
void row_log_table_insert(
    const rec_t *rec,       /*!< in: clustered index leaf page record,
                            page X-latched */
    const dtuple_t *ventry, /*!< in: dtuple holding virtual column info */
    dict_index_t *index,    /*!< in/out: clustered index, S-latched
                            or X-latched */
    const ulint *offsets)   /*!< in: rec_get_offsets(rec,index) */
    UNIV_COLD;
/** 记录 Online ALTER TABLE 期间 BLOB 被释放的事件。
用于跟踪 BLOB 页面的生命周期，确保回放时不会引用已释放的 BLOB。 */
void row_log_table_blob_free(
    dict_index_t *index, /*!< in/out: clustered index, X-latched */
    page_no_t page_no)   /*!< in: starting page number of the BLOB */
    UNIV_COLD;
/** 记录 Online ALTER TABLE 期间 BLOB 被分配的事件。 */
void row_log_table_blob_alloc(
    dict_index_t *index, /*!< in/out: clustered index, X-latched */
    page_no_t page_no)   /*!< in: starting page number of the BLOB */
    UNIV_COLD;

/** 将表级 row log 回放到重建完成的新表上。
这是 Online 表重建的最后一步（在 commit_try_rebuild 中调用）：
在持有排他锁的情况下，将 DDL 执行期间积累的所有并发 DML 变更
（INSERT/UPDATE/DELETE）逐条回放到新表的聚簇索引中。
如果 row log 过大（超过 innodb_online_alter_log_max_size），
返回 DB_ONLINE_LOG_TOO_BIG 错误。
@param[in]      thr             query graph
@param[in]      old_table       old table
@param[in,out]  table           MySQL table (for reporting duplicates)
@param[in,out]  stage           performance schema accounting object, used by
ALTER TABLE. stage->begin_phase_log_table() will be called initially and then
stage->inc() will be called for each block of log that is applied.
@return DB_SUCCESS, or error code on failure */
[[nodiscard]] dberr_t row_log_table_apply(que_thr_t *thr,
                                          dict_table_t *old_table,
                                          struct TABLE *table,
                                          Alter_stage *stage);

/** 获取在线创建索引期间调用 row_log_online_op() 的最新事务 ID。
用于确定 row log 中记录的最新事务，以便在回放时正确处理可见性。
 @return latest transaction ID, or 0 if nothing was logged */
[[nodiscard]] trx_id_t row_log_get_max_trx(
    dict_index_t *index); /*!< in: index, must be locked */

/** 将索引级 row log 回放到在线创建完成的二级索引上。
这是 Online 创建二级索引的最后一步（在 commit_try_norebuild 中调用）：
在持有排他锁的情况下，将索引创建期间积累的并发 DML 变更
逐条回放到新的二级索引中。
@param[in]      trx     transaction (for checking if the operation was
interrupted)
@param[in,out]  index   secondary index
@param[in,out]  table   MySQL table (for reporting duplicates)
@param[in,out]  stage   performance schema accounting object, used by
ALTER TABLE. stage->begin_phase_log_index() will be called initially and then
stage->inc() will be called for each block of log that is applied.
@return DB_SUCCESS, or error code on failure */
[[nodiscard]] dberr_t row_log_apply(const trx_t *trx, dict_index_t *index,
                                    struct TABLE *table, Alter_stage *stage);

#ifdef HAVE_PSI_STAGE_INTERFACE
/** Estimate how much work is to be done by the log apply phase
of an ALTER TABLE for this index.
@param[in]      index   index whose log to assess
@return work to be done by log-apply in abstract units
*/
ulint row_log_estimate_work(const dict_index_t *index);
#endif /* HAVE_PSI_STAGE_INTERFACE */

#include "row0log.ic"

#endif /* row0log.h */

/*
 * @copyright
 *
 *  Copyright 2016 Neo Natura
 *
 *  This file is part of the zio project.
 *  (https://github.com/neonatura/zio)
 *        
 *  The zio project is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version. 
 *
 *  The zio project is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with the zio project.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  @endcopyright
 */  

#ifndef __ZDB__ZDB_TABLE_H__
#define __ZDB__ZDB_TABLE_H__

#ifdef __cplusplus
extern "C" {
#endif
 

//#define ZDB_TABLE_SIZE 262144 /* 64bit */
#define ZDB_TABLE_SIZE 1048576 /* 4meg "tmp" table file */

#define ZDB_TABLE_POS_MASK 0xFFFFFFF0
#define ZDB_TABLE_NULL_POS 0xFFFFFFF1
#define ZDB_TABLE_SEARCH_POS 0x0

#define ZDB_TABLE_EXTENSION "tmp"


int zdb_table_find(zdb_t *db, zdb_hash_t hash, bcpos_t *ret_pos);

int zdb_table_set(zdb_t *db, zdb_hash_t hash, bcpos_t pos);


/** Remove a hash from the index table. */
int zdb_table_unset(zdb_t *db, zdb_hash_t hash);

/** Mark a index table hash as unknown. */
int zdb_table_reset(zdb_t *db, zdb_hash_t hash);

/** Obtain a reference to the index position table value */
bcpos_t *zdb_table_pos(zdb_t *db, zdb_hash_t hash);

/** Obtain a hash code identifying the table entry. */
int zdb_table_hash(zdb_hash_t hash);

/** Clear all hashes from the entire table. */
int zdb_table_clear(zdb_t *db);

int zdb_table_open(zdb_t *db);

void zdb_table_close(zdb_t *db);


#ifdef __cplusplus
}
#endif


#endif /* ndef __ZDB__ZDB_TABLE_H__ */


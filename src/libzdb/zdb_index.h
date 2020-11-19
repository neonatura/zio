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

#ifndef __ZDB__ZDB_INDEX_H__ 
#define __ZDB__ZDB_INDEX_H__ 

int zdb_idx_set(zdb_t *db, zdb_size_t pos, zdb_idx_t *idx);

int zdb_idx_clear(zdb_t *db, zdb_size_t pos);
int zdb_idx_find(zdb_t *db, zdb_hash_t hash, zdb_idx_t *ret_idx, int *ret_pos);
int zdb_idx_get(zdb_t *db, zdb_size_t pos, zdb_idx_t *ret_idx);

/**
 * @returns The next record index in the specified database.
 */
int zdb_idx_next(zdb_t *db, bcpos_t *pos_p);

int zdb_idx_reset(zdb_t *db, zdb_size_t pos, zdb_idx_t *idx);

int zdb_idx_open(zdb_t *db);

void zdb_idx_close(zdb_t *db);


#endif /* ndef __ZDB__ZDB_INDEX_H__  */

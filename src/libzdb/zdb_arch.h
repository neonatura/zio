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

#ifndef __ZDB__ZDB_ARCH_H__
#define __ZDB__ZDB_ARCH_H__


int zdb_arch_open(zdb_t *db);

void zdb_arch_close(zdb_t *db);

/**
 * Add a new "archived record" to the chain.
 */
int zdb_arch_add(zdb_t *db, zdb_idx_t *arch);

uint32_t zdb_arch_crc(zdb_hash_t hash);

int zdb_arch_get(zdb_t *db, bcpos_t pos, zdb_idx_t *ret_arch);

/**
 * The next record index available in the database. 
 */
int zdb_arch_next(zdb_t *db, bcpos_t *pos_p);

int zdb_arch_set(zdb_t *db, bcpos_t pos, zdb_idx_t *arch);

int zdb_arch_find(zdb_t *db, zdb_hash_t hash, zdb_idx_t *ret_arch, zdb_size_t *ret_pos);



#endif /* ndef __ZDB__ZDB_ARCH_H__ */


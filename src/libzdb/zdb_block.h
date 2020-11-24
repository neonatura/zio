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

#ifndef __ZDB__ZDB_BLOCK_H__
#define __ZDB__ZDB_BLOCK_H__

int zdb_open(char *name, zdb_t **zdb_p);

char *zdb_name(zdb_t *db);

int zdb_read(zdb_t *db, zdb_size_t pos, void *data, zdb_size_t data_len);

int zdb_append(zdb_t *db, zdb_hash_t hash, void *data, size_t data_len);

int zdb_find(zdb_t *db, zdb_hash_t hash, bcpos_t *pos_p);

int zdb_alloc(zdb_t *db, unsigned int jrnl);

int zdb_get_hash(zdb_t *db, bcpos_t pos, zdb_hash_t ret_hash);

int zdb_hash_cmp(zdb_hash_t a_hash, zdb_hash_t b_hash);

int zdb_idx_new(zdb_t *db, int pos, zdb_hash_t hash, zdb_size_t data_len);

void zdb_close(zdb_t *db);

/**
 * Move an index reference to the archived chain.
 */
int zdb_clear(zdb_t *db, zdb_size_t pos);

int zdb_write(zdb_t *db, zdb_size_t pos, zdb_hash_t hash, void *raw_data, int data_len);

int zdb_get(zdb_t *db, bcpos_t pos, unsigned char **data_p, size_t *data_len_p);

uint32_t zdb_journal(int pos);

int zdb_arch(zdb_t *db, zdb_size_t pos, unsigned char **data_p, size_t *data_len_p);

int zdb_purge(zdb_t *db, zdb_size_t pos);

int zdb_rewrite(zdb_t *db, zdb_size_t pos, zdb_hash_t hash, void *raw_data, int data_len);

const char *zdb_path_base(void);

int zdb_arch_write(zdb_t *db, zdb_hash_t hash, void *raw_data, zdb_size_t data_len);

int zdb_find(zdb_t *db, zdb_hash_t hash, bcpos_t *pos_p);

int zdb_set(zdb_t *db, zdb_size_t pos, void *raw_data, int data_len);

#endif /* ndef __ZDB__ZDB_BLOCK_H__ */

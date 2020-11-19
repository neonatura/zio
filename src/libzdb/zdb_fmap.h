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

#ifndef __ZDB__ZDB_FMAP_H__ 
#define __ZDB__ZDB_FMAP_H__ 

void zdb_map_free(zdb_map_t *map);

int zdb_map_open(zdb_t *db, zdb_map_t *map);

int zdb_map_alloc(zdb_t *db, zdb_map_t *map, zdb_size_t len);

int zdb_map_append(zdb_t *db, zdb_map_t *map, void *raw_data, zdb_size_t data_len);

int zdb_map_write(zdb_t *db, zdb_map_t *map, zdb_size_t of, void *raw_data, zdb_size_t data_len);

int zdb_map_trunc(zdb_t *db, zdb_map_t *map, zdb_size_t len);

unsigned int zdb_fmap_total(zdb_t *db);

void zdb_map_close(zdb_map_t *map);

#endif /* ndef __ZDB__ZDB_FMAP_H__  */

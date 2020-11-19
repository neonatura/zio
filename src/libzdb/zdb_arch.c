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

#include "zdb.h"

#define ZDB_ARCH_EXTENSION "arch"

static int _zdb_arch_open(zdb_t *db)
{
  int err;

  if (!db)
    return (ERR_INVAL);

  if (db->arch_map.fd != 0)
    return (0);

  /* set map file extension */
  strncpy(db->arch_map.ext, ZDB_ARCH_EXTENSION, sizeof(db->arch_map.ext)-1);

	err = zdb_map_open(db, &db->arch_map);
	if (err) {
		return (err);
	}

	err = zdb_map_alloc(db, &db->arch_map, 0);
	if (err) {
		return (err);
	}

  return (0);
}

int zdb_arch_open(zdb_t *db)
{
	int err;

	if (!zdb_lock(db))
		return (ERR_NOLCK);
	err = _zdb_arch_open(db);
	zdb_unlock(db);

	return (err);
}

static void _zdb_arch_close(zdb_t *db)
{

  if (db->arch_map.fd != 0) {
    zdb_map_close(&db->arch_map);
  }
  
}

void zdb_arch_close(zdb_t *db)
{

	if (zdb_lock(db)) {
		_zdb_arch_close(db);
		zdb_unlock(db);
	}
}

uint32_t zdb_arch_crc(zdb_hash_t hash)
{
  return (crc64(0, hash, sizeof(zdb_hash_t)));
}

int zdb_arch_find(zdb_t *db, zdb_hash_t hash, zdb_idx_t *ret_arch, zdb_size_t *ret_pos)
{
  zdb_hash_t t_hash;
  zdb_idx_t *arch;
  zdb_size_t len;
  int crc;
  int err;
  int i;

  err = zdb_arch_open(db);
  if (err)
    return (err);

  if (ret_arch)
    memset(ret_arch, '\000', sizeof(zdb_idx_t));
  if (ret_pos)
    *ret_pos = -1;

#if 0
  crc = zdb_arch_crc(hash);
#endif

  arch = (zdb_idx_t *)db->arch_map.raw;
  len = db->arch_map.hdr->of / sizeof(zdb_idx_t);
  for (i = (len-1); i >= 0; i--) {
    if (arch[i].size == 0) continue;
#if 0
    if (arch[i].crc != crc)
      continue; /* hash checksum does not match */    

    err = zdb_read(db, i, t_hash, sizeof(t_hash));
    if (err)
      return (err);

    if (zdb_hash_cmp(hash, t_hash)) {
      if (ret_arch)
        memcpy(ret_arch, arch + i, sizeof(zdb_idx_t));
      if (ret_pos)
        *ret_pos = i;
      return (0);
    }
#endif
    if (zdb_hash_cmp(hash, arch[i].hash)) {
      if (ret_arch)
        memcpy(ret_arch, arch + i, sizeof(zdb_idx_t));
      if (ret_pos)
        *ret_pos = i;
      return (0);
    }
  }

  return (ERR_NOENT);
}

static int _zdb_arch_get(zdb_t *db, bcpos_t pos, zdb_idx_t *ret_arch)
{
  zdb_idx_t *arch;
  int err;

  if (!db)
    return (ERR_INVAL);

  err = zdb_arch_open(db);
  if (err)
    return (err);

  if (pos >= (db->arch_map.hdr->of / sizeof(zdb_idx_t)))
    return (ERR_NOENT);

  arch = (zdb_idx_t *)db->arch_map.raw;
  if (arch[pos].size == 0)
    return (ERR_NOENT); /* not filled in */

  if (ret_arch) {
    memcpy(ret_arch, arch + pos, sizeof(zdb_idx_t));
  }

  return (0);
}

int zdb_arch_get(zdb_t *db, bcpos_t pos, zdb_idx_t *ret_arch)
{
	int err;

	if (!zdb_lock(db))
		return (ERR_NOLCK);
	err = _zdb_arch_get(db, pos, ret_arch);
	zdb_unlock(db);

	return (err);
}

/**
 * @returns The next record index.
 */
int zdb_arch_next(zdb_t *db, bcpos_t *pos_p)
{
  zdb_idx_t *arch;
  int err;

  err = zdb_arch_open(db);
  if (err)
    return (err);

	if (pos_p)
		*pos_p = db->arch_map.hdr->of / sizeof(zdb_idx_t);

	return (0);
}

/**
 * @todo consider clearing indexes which are brought back into main chain.
 */
static int _zdb_arch_set(zdb_t *db, bcpos_t pos, zdb_idx_t *arch)
{
  zdb_idx_t *f_arch;
  zdb_size_t of;
  int err;

  if (!db) {
    return (ERR_INVAL);
  }

  err = zdb_arch_open(db);
  if (err) {
    return (err);
  }

  if (zdb_arch_get(db, pos, NULL) != ERR_NOENT)
    return (ERR_EXIST);

  of = (pos * sizeof(zdb_idx_t));
  if (pos >= (db->arch_map.hdr->of / sizeof(zdb_idx_t)) &&
      (of + sizeof(zdb_idx_t)) > db->arch_map.size) {
    zdb_size_t f_len = (of + sizeof(zdb_idx_t)) - db->arch_map.size;

    err = zdb_map_alloc(db, &db->arch_map, f_len);
    if (err)
      return (err);
  }

  /* write to file map */
  err = zdb_map_write(db, &db->arch_map, of, arch, sizeof(zdb_idx_t)); 
  if (err)
    return (err);

  return (0);
}

int zdb_arch_set(zdb_t *db, bcpos_t pos, zdb_idx_t *arch)
{
	int err;

	if (!zdb_lock(db))
		return (ERR_NOLCK);
	err = _zdb_arch_set(db, pos, arch);
	zdb_unlock(db);

	return (err);
}

int zdb_arch_add(zdb_t *db, zdb_idx_t *arch)
{
	bcpos_t pos;
	int err;

	err = zdb_arch_next(db, &pos);
	if (err)
		return (err);

  return (zdb_arch_set(db, pos, arch));
}


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

#define ZDB_INDEX_EXTENSION "idx"

static int _zdb_idx_open(zdb_t *db)
{
  char errbuf[256];
  int err;

  if (!db)
    return (ERR_INVAL);

  if (db->idx_map.fd != 0) {
    return (0);
  }

  /* set map file extension */
  strncpy(db->idx_map.ext, ZDB_INDEX_EXTENSION, sizeof(db->idx_map.ext)-1);

  err = zdb_map_open(db, &db->idx_map);
  if (err) {
    sprintf(errbuf, "zdb_idx_open: map open error.");
    zlog_error(err, errbuf);
    return (err);
  }

  err = zdb_map_alloc(db, &db->idx_map, 0);
  if (err) {
    sprintf(errbuf, "zdb_idx_open: map alloc error.");
    zlog_error(err, errbuf);
    return (err);
  }

  return (0);
}

int zdb_idx_open(zdb_t *db)
{
  int err;

	if (!db)
		return (ERR_INVAL);

  if (!zdb_lock(db))
		return (ERR_NOLCK);
  err = _zdb_idx_open(db);
  zdb_unlock(db);

  return (err);
}

static void _zdb_idx_close(zdb_t *db)
{

  if (db->idx_map.fd != 0) {
    zdb_map_close(&db->idx_map);
  }
  
}

void zdb_idx_close(zdb_t *db)
{

  if (zdb_lock(db)) {
		_zdb_idx_close(db);
		zdb_unlock(db);
	}

}

static int _zdb_idx_find(zdb_t *db, zdb_hash_t hash, zdb_idx_t *ret_idx, int *ret_pos)
{
  zdb_hash_t t_hash;
  zdb_idx_t *idx;
  zdb_size_t len;
  char errbuf[256];
  uint64_t pos;
  int pos_high;
  int tab_hash;
  int crc;
  int err;
  int i;

  if (ret_idx)
    memset(ret_idx, '\000', sizeof(zdb_idx_t));
  if (ret_pos)
    *ret_pos = -1;

  pos = 0;
  pos_high = -1;
  err = zdb_table_get(db, hash, &pos);
  if (err == ERR_NOENT)
    return (ERR_NOENT);
  if (err == 0) {
    /* current highest known position for table hash */
    pos_high = (int)pos;
  }

  err = zdb_idx_open(db);
  if (err) {
    sprintf(errbuf, "zdb_idx_find: error opening database.");
    zlog_error(err, errbuf);
    return (err);
  }

  idx = (zdb_idx_t *)db->idx_map.raw;

  len = (db->idx_map.hdr->of / sizeof(zdb_idx_t)) - 1;
  if (pos_high > len)
    pos_high = -1;
  if (pos_high != -1)
    len = pos_high;

  tab_hash = zdb_table_hash(hash);
  for (i = len; i >= 0; i--) {
    if (idx[i].size == 0) continue;

		if (i > pos_high &&
				zdb_table_hash(idx[i].hash) == tab_hash)
      pos_high = i;

    if (zdb_hash_cmp(hash, idx[i].hash))
      break;
  }

  if (pos_high != -1) {
    if (pos != (uint64_t)pos_high) {
      /* record highest index pos for table hash. */
      zdb_table_set(db, hash, pos_high);
    }
  } else if (i < 0) {
    /* no entries found and no indexes matched table hash */
    zdb_table_unset(db, hash);
  }

  if (i < 0) {
    /* no index position found. */
    return (ERR_NOENT);
  }

  if (ret_idx)
    memcpy(ret_idx, idx + i, sizeof(zdb_idx_t));
  if (ret_pos)
    *ret_pos = i;

  return (0);
}

int zdb_idx_find(zdb_t *db, zdb_hash_t hash, zdb_idx_t *ret_idx, int *ret_pos)
{
  int err;

  if (!zdb_lock(db))
		return (ERR_NOLCK);
  err = _zdb_idx_find(db, hash, ret_idx, ret_pos);
  zdb_unlock(db);

  return (err);
}

static int _zdb_idx_get(zdb_t *db, zdb_size_t pos, zdb_idx_t *ret_idx)
{
  zdb_idx_t *idx;
  int err;

  if (!db)
    return (ERR_INVAL);

  if (pos < 0)
    return (ERR_INVAL);

  err = zdb_idx_open(db);
  if (err)
    return (err);

  if (pos >= (db->idx_map.hdr->of / sizeof(zdb_idx_t)))
    return (ERR_NOENT);

  idx = (zdb_idx_t *)db->idx_map.raw;
  if (idx[pos].size == 0)
    return (ERR_NOENT); /* not filled in */

  if (ret_idx) {
    memcpy(ret_idx, idx + pos, sizeof(zdb_idx_t));
  }

  return (0);
}

int zdb_idx_get(zdb_t *db, zdb_size_t pos, zdb_idx_t *ret_idx)
{
  int err;

  if (!zdb_lock(db))
		return (ERR_NOLCK);
  err = _zdb_idx_get(db, pos, ret_idx);
  zdb_unlock(db);

  return (err);

}

static int _zdb_idx_set(zdb_t *db, zdb_size_t pos, zdb_idx_t *idx)
{
  zdb_idx_t *f_idx;
  zdb_size_t of;
  int err;

  if (!db || pos < 0) {
    return (ERR_INVAL);
  }

  err = zdb_idx_open(db);
  if (err) {
    return (err);
  }

#if 0
  if (pos == 0) {
    zdb_idx_t blank_idx;

    /* blank initial record */
    memset(&blank_idx, 0, sizeof(blank_idx));
    err = zdb_map_append(db, &db->idx_map, &blank_idx, sizeof(zdb_idx_t));
    if (err)
      return (err);

    pos++;
  }
#endif

  if (zdb_idx_get(db, pos, NULL) != ERR_NOENT)
    return (ERR_EXIST);

  of = (pos * sizeof(zdb_idx_t));
  if (pos >= (db->idx_map.hdr->of / sizeof(zdb_idx_t)) &&
      (of + sizeof(zdb_idx_t)) > db->idx_map.size) {
    zdb_size_t f_len = (of + sizeof(zdb_idx_t)) - db->idx_map.size;

    err = zdb_map_alloc(db, &db->idx_map, f_len);
    if (err) {
      return (err);
    }
  }

  /* write to file map */
  err = zdb_map_write(db, &db->idx_map, of, idx, sizeof(zdb_idx_t)); 
  if (err)
    return (err);

  /* mark table hash entry as requiring new search. */
  zdb_table_reset(db, idx->hash);

  return (0);
}

int zdb_idx_set(zdb_t *db, zdb_size_t pos, zdb_idx_t *idx)
{
  int err;

  if (!zdb_lock(db))
		return (ERR_NOLCK);
  err = _zdb_idx_set(db, pos, idx);
  zdb_unlock(db);

  return (err);
}

static int _zdb_idx_reset(zdb_t *db, zdb_size_t pos, zdb_idx_t *idx)
{
  zdb_idx_t n_idx;
  zdb_size_t of;
  int err;

  if (!db || pos < 0) {
    return (ERR_INVAL);
  }

  err = zdb_idx_open(db);
  if (err) {
    return (err);
  }

#if 0
  if (pos == 0) {
    zdb_idx_t blank_idx;

    /* blank initial record */
    memset(&blank_idx, 0, sizeof(blank_idx));
    err = zdb_map_append(db, &db->idx_map, &blank_idx, sizeof(zdb_idx_t));
    if (err)
      return (err);

    pos++;
  }
#endif

  if (zdb_idx_get(db, pos, &n_idx) != 0)
    return (ERR_NOENT);

  of = (pos * sizeof(zdb_idx_t));
  if (pos >= (db->idx_map.hdr->of / sizeof(zdb_idx_t)) ||
      (of + sizeof(zdb_idx_t)) > db->idx_map.size) {
    return (ERR_IO);
  }

  /* write to file map */
  err = zdb_map_write(db, &db->idx_map, of, idx, sizeof(zdb_idx_t)); 
  if (err)
    return (err);

  return (0);
}

int zdb_idx_reset(zdb_t *db, zdb_size_t pos, zdb_idx_t *idx)
{
  int err;

  if (!zdb_lock(db))
		return (ERR_NOLCK);
  err = _zdb_idx_reset(db, pos, idx);
  zdb_unlock(db);

  return (err);
}

/**
 * @note (odd) only reduces index count when "pos" is last record in db.
 */
static int _zdb_idx_clear(zdb_t *db, zdb_size_t pos)
{
  zdb_idx_t *f_idx;
  zdb_idx_t blank_idx;
  zdb_size_t n_pos;
  zdb_size_t of;
  int err;

  if (!db || pos < 0) {
    return (ERR_INVAL);
  }

  err = zdb_idx_open(db);
  if (err)
    return (err);

  n_pos = MAX(0, (db->idx_map.hdr->of / sizeof(zdb_idx_t)));

  of = (pos * sizeof(zdb_idx_t));
  if (pos >= (db->idx_map.hdr->of / sizeof(zdb_idx_t)) &&
      (of + sizeof(zdb_idx_t)) > db->idx_map.size) {
    /* no content - does not exist */
  } else {
    /* write to file map */
    memset(&blank_idx, 0, sizeof(blank_idx));
    err = zdb_map_write(db, &db->idx_map, of, &blank_idx, sizeof(zdb_idx_t)); 
    if (err)
      return (err);
  }

  return (0);
}

int zdb_idx_clear(zdb_t *db, zdb_size_t pos)
{
  int err;

  if (!zdb_lock(db))
		return (ERR_NOLCK);
  err = _zdb_idx_clear(db, pos);
  zdb_unlock(db);

  return (err);
}

int zdb_idx_next(zdb_t *db, bcpos_t *pos_p)
{
  zdb_idx_t *idx;
  int err;

  err = zdb_idx_open(db);
  if (err)
    return (err);

	if (pos_p)
		*pos_p = (db->idx_map.hdr->of / sizeof(zdb_idx_t));

	return (0);
}

uint32_t zdb_idx_crc(zdb_hash_t hash)
{
  return (crc64(0, hash, sizeof(zdb_hash_t)) & 0xFFFFFFFF);
}


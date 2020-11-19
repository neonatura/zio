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

int zdb_table_hash(zdb_hash_t hash)
{
  int i;
  uint32_t *p;
  uint32_t val;

  val = 0;
  p = (uint32_t *)hash;
  for (i = 0; i < 8; i++)
    val += p[i];

  return ((int)(val % ZDB_TABLE_SIZE));
}

bcpos_t *zdb_table_pos(zdb_t *db, zdb_hash_t hash)
{
  bcpos_t *tab;
  int idx;

  tab = (bcpos_t *)db->tab_map.raw;
  if (!tab)
    return (NULL);

  idx = zdb_table_hash(hash);
  return (&tab[idx]);
}

static int _zdb_table_get(zdb_t *db, zdb_hash_t hash, bcpos_t *ret_pos)
{
  bcpos_t *pos_p;
  int err;
  int i;

  err = zdb_table_open(db);
  if (err)
    return (err);

  if (ret_pos)
    *ret_pos = ZDB_TABLE_NULL_POS; 

  pos_p = zdb_table_pos(db, hash);
  if (!pos_p)
    return (ERR_INVAL);
  if (*pos_p == ZDB_TABLE_SEARCH_POS)
    return (ERR_SRCH);
  if (*pos_p == ZDB_TABLE_NULL_POS)
    return (ERR_NOENT); /* no record exists */

  if (ret_pos) {
    *ret_pos = *pos_p;
  }

  return (0);  
}

int zdb_table_get(zdb_t *db, zdb_hash_t hash, bcpos_t *ret_pos)
{
  int err;

  if (!zdb_lock(db))
		return (ERR_NOLCK);
  err = _zdb_table_get(db, hash, ret_pos);
  zdb_unlock(db);

  return (err);
}

static int _zdb_table_set(zdb_t *db, zdb_hash_t hash, bcpos_t pos)
{
  bcpos_t *pos_p;
  uint8_t a, b;
  int err;

  if (!db || pos >= ZDB_TABLE_POS_MASK)
    return (ERR_INVAL);

  err = zdb_table_open(db);
  if (err)
    return (err);

  pos_p = zdb_table_pos(db, hash);
  if (!pos_p)
    return (ERR_INVAL);

  if (*pos_p < pos || *pos_p >= ZDB_TABLE_POS_MASK) {
    /* retain highest index position for hash entry */
    *pos_p = pos;
  }

  return (0);
}

int zdb_table_set(zdb_t *db, zdb_hash_t hash, bcpos_t pos)
{
  int err;

  if (!zdb_lock(db))
		return (ERR_NOLCK);
  err = _zdb_table_set(db, hash, pos);
  zdb_unlock(db);

  return (err);
}

static int _zdb_table_unset(zdb_t *db, zdb_hash_t hash)
{
  bcpos_t *pos_p;
  uint8_t a, b;
  int err;

  if (!db)
    return (ERR_INVAL);

  err = zdb_table_open(db);
  if (err)
    return (err);

  pos_p = zdb_table_pos(db, hash);
  if (!pos_p)
    return (ERR_INVAL);

  *pos_p = ZDB_TABLE_NULL_POS;

  return (0);
}

int zdb_table_unset(zdb_t *db, zdb_hash_t hash)
{
  int err;

  if (!zdb_lock(db))
		return (ERR_NOLCK);
  err = _zdb_table_unset(db, hash);
  zdb_unlock(db);

  return (err);
}

static int _zdb_table_reset(zdb_t *db, zdb_hash_t hash)
{
  bcpos_t *pos_p;
  uint8_t a, b;
  int err;

  if (!db)
    return (ERR_INVAL);

  err = zdb_table_open(db);
  if (err)
    return (err);

  pos_p = zdb_table_pos(db, hash);
  if (!pos_p)
    return (ERR_INVAL);

  
  *pos_p = ZDB_TABLE_SEARCH_POS;

  return (0);
}

int zdb_table_reset(zdb_t *db, zdb_hash_t hash)
{
  int err;

  if (!zdb_lock(db))
		return (ERR_NOLCK);
  err = _zdb_table_reset(db, hash);
  zdb_unlock(db);

  return (err);
}

static int _zdb_table_open(zdb_t *db)
{
  char errbuf[256];
  int err;

  if (!db)
    return (ERR_INVAL);

  if (db->tab_map.fd != 0) {
    return (0);
  }

  /* set map file extension */
  strncpy(db->tab_map.ext, ZDB_TABLE_EXTENSION, sizeof(db->tab_map.ext)-1);

  err = zdb_map_open(db, &db->tab_map);
  if (err) {
    sprintf(errbuf, "zdb_table_open: map open error.");
    zlog_error(err, errbuf);
    return (err);
  }

  err = zdb_map_alloc(db, &db->tab_map, (ZDB_TABLE_SIZE * sizeof(bcpos_t)));
  if (err) {
    sprintf(errbuf, "zdb_table_open: map alloc error.");
    zlog_error(err, errbuf);
    return (err);
  }

  return (0);
}

int zdb_table_open(zdb_t *db)
{
  int err;

  if (!zdb_lock(db))
		return (ERR_NOLCK);
  err = _zdb_table_open(db);
  zdb_unlock(db);

  return (err);
}

static void _zdb_table_close(zdb_t *db)
{

  if (db->tab_map.fd != 0) {
    zdb_map_close(&db->tab_map);
  }
  
}

void zdb_table_close(zdb_t *db)
{

  if (zdb_lock(db)) {
		_zdb_table_close(db);
		zdb_unlock(db);
	}

}

static int _zdb_table_clear(zdb_t *db)
{
  int err;

  err = zdb_table_open(db);
  if (err)
    return (err);

  /* mark entire hash-map table as 'search required'. */
  memset(db->tab_map.raw, '\000', db->tab_map.size - sizeof(zdb_hdr_t));

  return (0);
}

int zdb_table_clear(zdb_t *db)
{
  int err;

  if (!zdb_lock(db))
		return (ERR_NOLCK);
  err = _zdb_table_clear(db);
  zdb_unlock(db);

  return (err);
}

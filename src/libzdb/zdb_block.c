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

#define BCMAP_IDLE_TIME 240

char *zdb_name(zdb_t *db)
{

  if (!db)
    return (NULL);

  return (db->name);
}


static int _zdb_open(char *name, zdb_t **zdb_p)
{
  zdb_t *db;
  int err;

  if (!zdb_p)
    return (ERR_INVAL);

  db = (zdb_t *)calloc(1, sizeof(zdb_t));
  if (!db)
    return (ERR_NOMEM);

  strncpy(db->name, name, sizeof(db->name) - 1);

	zdb_mutex_init(db);

#if 0
	/* wait until needed.. */
  err = zdb_idx_open(db);
  if (err)
    return (err);
#endif

  *zdb_p = db;

  return (0);
}

int zdb_open(char *name, zdb_t **zdb_p)
{
  return (_zdb_open(name, zdb_p));
}

static void _zdb_close(zdb_t *db)
{
  zdb_map_t *map;
  int i;

	/* wait for any pending access */
	(void)zdb_lock(db);

	/* close index map */
	zdb_idx_close(db);

	/* close hash table */
	zdb_table_close(db);

	/* close data maps */
	for (i = 0; i < db->data_map_len; i++) {
		map = db->data_map + i;
		zdb_map_close(map);
	}

	/* free data map list */
	free(db->data_map);
	db->data_map = NULL;
	db->data_map_len = 0;

	/* unlock and destroy mutex */
	zdb_unlock(db);
	zdb_mutex_term(db);

  free(db);
}

void zdb_close(zdb_t *db)
{

	if (!db)
		return;

	_zdb_close(db);
	
}

/**
 * @todo auto de-alloc maps that expire
 */
int zdb_alloc(zdb_t *db, unsigned int jrnl)
{
  zdb_map_t *map;
  char ext[64];
  int err;

  if (jrnl >= db->data_map_len) {
    zdb_map_t *o_data_map;
    zdb_map_t *n_data_map;

    o_data_map = db->data_map;
    n_data_map = (zdb_map_t *)calloc(jrnl+1, sizeof(zdb_map_t));
    if (o_data_map) {
      memcpy(n_data_map, o_data_map, db->data_map_len * sizeof(zdb_map_t));
      free(o_data_map);
    }

    db->data_map = n_data_map;
    db->data_map_len = jrnl+1;
  }

  map = db->data_map + jrnl;

  if (!*map->ext) {
    sprintf(map->ext, "%u", jrnl);
  }

  err = zdb_map_alloc(db, map, 0);
  if (err)
    return (err);

  return (0);
}

#if 0
int zdb_write(zdb_t *db, unsigned int jrnl, unsigned char *data, int data_len)
{
  zdb_map_t *map;
  char ext[64];
  int err;

  err = zdb_alloc(db, jrnl);
  if (err)
    return (err);

  map = db->data_map + jrnl;
  if (!*map->ext) {
    sprintf(map->ext, "%u", jrnl);
  }

  /* serialized block data */
  err = zdb_map_append(db, map, data, data_len);
  if (err)
    return (err);

  return (0);
}
#endif

static int _zdb_rewrite(zdb_t *db, zdb_size_t pos, zdb_hash_t hash, void *raw_data, int data_len)
{
  unsigned char *data = (unsigned char *)raw_data;
  zdb_idx_t idx;
  zdb_map_t *map;
  char ext[64];
  char errbuf[256];
  int jrnl;
  int err;

  jrnl = zdb_journal(pos);

  err = zdb_alloc(db, jrnl);
  if (err) {
    sprintf(errbuf, "zdb_write: error: zdb_alloc failed.");
    zlog_error(err, errbuf);
    return (err);
  }

  map = db->data_map + jrnl;
  if (!*map->ext)
    sprintf(map->ext, "%u", idx.jrnl);

  err = zdb_idx_get(db, pos, &idx);
  if (err)
    return (err);

  if (idx.size < data_len)
    return (ERR_EXIST);

  idx.size = data_len;
  idx.crc = (crc64(0, data, data_len) & 0xFFFFFFFF); 

  /* store fresh block index */
  err = zdb_idx_reset(db, pos, &idx);
  if (err) { 
    sprintf(errbuf, "zdb_write: error: zdb_idx_set failure.");
    zlog_error(err, errbuf);
    return (err);
  }

  /* store serialized block data */
  err = zdb_map_write(db, map, idx.of, data, data_len);
  if (err) { /* uh oh */
    sprintf(errbuf, "zdb_write: error: zdb_map_append failure.");
    zlog_error(err, errbuf);
    zdb_idx_clear(db, pos);
    zdb_table_reset(db, hash);
    return (err);
  }

  return (0);
}

int zdb_rewrite(zdb_t *db, zdb_size_t pos, zdb_hash_t hash, void *raw_data, int data_len)
{
  int err;

  if (!zdb_lock(db))
    return (ERR_NOLCK);
  err = _zdb_rewrite(db, pos, hash, raw_data, data_len);
  zdb_unlock(db);

  return (err);
}

static int _zdb_write(zdb_t *db, zdb_size_t pos, zdb_hash_t hash, void *raw_data, int data_len)
{
  unsigned char *data = (unsigned char *)raw_data;
  zdb_idx_t idx;
  zdb_map_t *map;
  char ext[64];
  char errbuf[256];
  int jrnl;
  int err;

  jrnl = zdb_journal(pos);

  err = zdb_alloc(db, jrnl);
  if (err) {
    sprintf(errbuf, "zdb_write: error: zdb_alloc failed.");
    zlog_error(err, errbuf);
    return (err);
  }

  map = db->data_map + jrnl;
  if (!*map->ext)
    sprintf(map->ext, "%u", idx.jrnl);

  /* finialize block index */
  memset(&idx, 0, sizeof(idx));
  idx.jrnl = jrnl;
  idx.size = data_len;
  idx.of = map->hdr->of;
  idx.crc = (crc64(0, data, data_len) & 0xFFFFFFFF);
  memcpy(idx.hash, hash, sizeof(zdb_hash_t));

  /* store fresh block index */
  err = zdb_idx_set(db, pos, &idx);
  if (err) { 
    if (err != ERR_EXIST) {
      sprintf(errbuf, "zdb_write: error: zdb_idx_set failure.");
      zlog_error(err, errbuf);
			return (err);
		}

		/* record already existed */
		err = _zdb_rewrite(db, pos, hash, raw_data, data_len); 
    if (err) {
      sprintf(errbuf, "zdb_write: error: zdb_rewrite failure.");
      zlog_error(err, errbuf);
      return (err);
		}
  } else {
    /* store serialized block data */
    err = zdb_map_append(db, map, data, data_len);
    if (err) { /* uh oh */
      sprintf(errbuf, "zdb_write: error: zdb_map_append failure.");
      zlog_error(err, errbuf);
      zdb_idx_clear(db, pos);
      zdb_table_reset(db, hash);
      return (err);
    }
  }

  return (0);
}

int zdb_write(zdb_t *db, zdb_size_t pos, zdb_hash_t hash, void *raw_data, int data_len)
{
  int err;

  if (!zdb_lock(db))
    return (ERR_NOLCK);
  err = _zdb_write(db, pos, hash, raw_data, data_len);
  zdb_unlock(db);

  return (err);
}

int zdb_set(zdb_t *db, zdb_size_t pos, void *raw_data, int data_len)
{
	static zdb_hash_t hash;
	return (zdb_write(db, pos, hash, raw_data, data_len));
}

/**
 * @returns The new record position or a negative error code.
 */
static int _zdb_append(zdb_t *db, zdb_hash_t hash, void *data, size_t data_len)
{
  unsigned char *raw_data = (unsigned char *)data;
  zdb_idx_t idx;
  zdb_map_t *map;
  char errbuf[256];
	bcpos_t pos;
  int err;

  err = zdb_idx_open(db);
  if (err) {
    sprintf(errbuf, "zdb_append: error opening map index.");
    zlog_error(err, errbuf);
    return (err);
  }

	err = zdb_idx_next(db, &pos);
	if (err) {
    sprintf(errbuf, "zdb_append: error positioning map index.");
    zlog_error(err, errbuf);
    return (err);
  }

  err = zdb_write(db, pos, hash, data, data_len);
  if (err) {
    sprintf(errbuf, "zdb_append: write error.");
    zlog_error(err, errbuf);
    return (err); 
  }

  return (0);
}

int zdb_append(zdb_t *db, zdb_hash_t hash, void *data, size_t data_len)
{
  int err;

  if (!zdb_lock(db))
		return (ERR_NOLCK);
  err = _zdb_append(db, hash, data, data_len);
  zdb_unlock(db);

  return (err);
}


/**
 * Fills a pre-allocated binary segment with a specified size from a specified record position.
 */
static int _zdb_read(zdb_t *db, zdb_size_t pos, void *data, zdb_size_t data_len)
{
  zdb_map_t *map;
  zdb_idx_t idx;
  char errbuf[256];
  int err;

  /* obtain index for record position */
  memset(&idx, 0, sizeof(idx));
  err = zdb_idx_get(db, pos, &idx);
  if (err)
    return (err);

  /* ensure journal is allocated */
  err = zdb_alloc(db, idx.jrnl);
  if (err) {
    sprintf(errbuf, "zdb_read: error allocating journal (%s).", db->name);
    zlog_error(err, errbuf);
    return (err);
  }

  memset(data, 0, data_len);
  data_len = MIN(data_len, idx.size); 
  
  map = db->data_map + idx.jrnl;
	if (idx.of >= map->size) {
    sprintf(errbuf, "zdb_read: error: invalid record offset %u [max %u].", idx.of, map->size);
    zlog_error(ERR_ILSEQ, errbuf);
		return (ERR_IO);
	}

  if ((crc64(0, map->raw + idx.of, idx.size) & 0xFFFFFFFF) != idx.crc) {
    sprintf(errbuf, "zdb_read: error: invalid crc {map: %x, idx: %x} mismatch at pos %d.", (crc64(0, map->raw + idx.of, idx.size) & 0xFFFFFFFF), idx.crc, pos);
    zlog_error(ERR_ILSEQ, errbuf);
    return (ERR_ILSEQ);
  }

  memcpy(data, map->raw + idx.of, data_len);

  return (0);
}

int zdb_read(zdb_t *db, zdb_size_t pos, void *data, zdb_size_t data_len)
{
  int err;
  int lk;

  if (!zdb_lock(db))
    return (ERR_NOLCK);
  err = _zdb_read(db, pos, data, data_len);
  zdb_unlock(db);

  return (err);
}

/**
 * Obtains an allocated binary segment stored at the specified record position. 
 */
static int _zdb_get(zdb_t *db, bcpos_t pos, unsigned char **data_p, size_t *data_len_p)
{
  zdb_idx_t idx;
  unsigned char *data;
  char errbuf[1024];
  int err;

  if (!data_p) {
    sprintf(errbuf, "zdb_get: error: no data pointer specified.");
    zlog_error(err, errbuf);
    return (ERR_INVAL);
  }

  /* obtain index for record position */
  memset(&idx, 0, sizeof(idx));
  err = zdb_idx_get(db, pos, &idx);
  if (err) {
    sprintf(errbuf, "zdb_get: error obtaining index [pos %d].", (int)pos);
    zlog_error(err, errbuf);
    return (err);
  }

/* .. deal with idx.size == 0, i.e. prevent write of 0 */
  data = (unsigned char *)malloc(MAX(1, idx.size));
  if (!data)
    return (ERR_NOMEM);

  /* read in serialized binary data */
  memset(data, '\000', idx.size);
  err = zdb_read(db, pos, data, idx.size);
  if (err) {
    sprintf(errbuf, "zdb_get: zdb_read <%d bytes> error [pos %d].", idx.size, (int)pos);
    zlog_error(err, errbuf);
    free(data);
    return (err); 
  }

  *data_p = data;
  if (data_len_p)
    *data_len_p = idx.size;

  return (0);
}

int zdb_get(zdb_t *db, bcpos_t pos, unsigned char **data_p, size_t *data_len_p)
{
  int err;

  if (!zdb_lock(db))
		return (ERR_NOLCK);
  err = _zdb_get(db, pos, data_p, data_len_p);
  zdb_unlock(db);

  return (err);
}

static int _zdb_arch(zdb_t *db, zdb_size_t pos, unsigned char **data_p, size_t *data_len_p)
{
  zdb_idx_t idx;
  zdb_map_t *map;
  unsigned char *data;
  size_t data_len;
  char errbuf[256];
  int err;

  if (!data_p) {
    sprintf(errbuf, "zdb_arch: no data pointer specified.");
    zlog_error(err, errbuf);
    return (ERR_INVAL);
  }

  *data_p = NULL;
  if (data_len_p)
    *data_len_p = 0;

  /* obtain index for record position */
  memset(&idx, 0, sizeof(idx));
  err = zdb_arch_get(db, pos, &idx);
  if (err) {
    sprintf(errbuf, "zdb_arch_get: zdb_idx_get error [pos %d].", pos);
    zlog_error(err, errbuf);
    return (err);
  }

  data_len = idx.size;
  data = (unsigned char *)calloc(data_len, sizeof(char)); 
  if (!data)
    return (ERR_NOMEM);

  /* ensure journal is allocated */
  err = zdb_alloc(db, idx.jrnl);
  if (err) {
    free(data);
    return (err);
  }

  /* read in serialized binary data */
  memset(data, 0, data_len);
  map = db->data_map + idx.jrnl;

  if ((crc64(0, map->raw + idx.of, idx.size) & 0xFFFFFFFF) != idx.crc) {
    sprintf(errbuf, "zdb_arch: error: invalid crc {map: %x, idx: %x} mismatch at pos %d\n", (crc64(0, map->raw + idx.of, idx.size) & 0xFFFFFFFF), idx.crc, pos);
    zlog_error(ERR_ILSEQ, errbuf);
    return (ERR_ILSEQ);
  }

  memcpy(data, map->raw + idx.of, data_len);

  *data_p = data;
  if (data_len_p)
    *data_len_p = idx.size;

  return (0);
}

int zdb_arch(zdb_t *db, zdb_size_t pos, unsigned char **data_p, size_t *data_len_p)
{
	int err;

	if (!zdb_lock(db))
		return (ERR_NOLCK);
	err = _zdb_arch(db, pos, data_p, data_len_p);
	zdb_unlock(db);

	return (err);
}

int zdb_get_hash(zdb_t *db, bcpos_t pos, zdb_hash_t ret_hash)
{
  zdb_idx_t idx;
  int err;

  /* obtain index for record position */
  err = zdb_idx_get(db, pos, &idx);
  if (err)
    return (err);

  memcpy(ret_hash, idx.hash, sizeof(zdb_hash_t));
  return (0);
}

/** 
 * Obtains the record position for a particular hash.
 */
int zdb_find(zdb_t *db, zdb_hash_t hash, bcpos_t *pos_p)
{
  int err;

  err = zdb_idx_find(db, hash, NULL, pos_p);
  if (err)
    return (err);
  

  return (0);
}


/**
 * @bug this does not handle jrnls alloc'd past one being targeted.
 */
static int _zdb_purge(zdb_t *db, zdb_size_t pos)
{
  zdb_map_t *map;
  zdb_idx_t idx;
  zdb_idx_t t_idx;
  char errbuf[256];
	bcpos_t max_rec;
  int err;
  int i;

  if (pos < 0)
    return (ERR_INVAL);

  /* obtain index for record position */
  memset(&idx, 0, sizeof(idx));
  err = zdb_idx_get(db, pos, &idx);
  if (err)
    return (err);

  /* ensure journal is allocated */
  err = zdb_alloc(db, idx.jrnl);
  if (err)
    return (err);

  map = db->data_map + idx.jrnl;

  if ((crc64(0, map->raw + idx.of, idx.size) & 0xFFFFFFFF) != idx.crc) {
    sprintf(errbuf, "zdb_purge: invalid crc {map: %x, idx: %x} mismatch at pos %d\n", (crc64(0, map->raw + idx.of, idx.size) & 0xFFFFFFFF), idx.crc, pos);
    zlog_error(err, errbuf);
    return (ERR_ILSEQ);
  }

  /* clear index of remaining entries */
	max_rec = 0;
	zdb_idx_next(db, &max_rec);
	if (max_rec) {
		bcpos_t t_pos;

		for (t_pos = (max_rec-1); t_pos >= pos; t_pos--) {
			zdb_clear(db, t_pos);

			err = zdb_idx_get(db, t_pos, &t_idx);
			if (!err) {
				zdb_table_reset(db, t_idx.hash);
			}
		}
	}

#if 0
  /* truncate journal at offset */
  zdb_map_trunc(db, map, idx.of);
#endif

  return (0);
}

int zdb_purge(zdb_t *db, zdb_size_t pos)
{
  int err;

  if (!zdb_lock(db))
    return (ERR_NOLCK);
  err = _zdb_purge(db, pos);
  zdb_unlock(db);

  return (err);
}


/**
 * @returns TRUE if the hashes are identical and FALSE otherwise.
 */
int zdb_hash_cmp(zdb_hash_t a_hash, zdb_hash_t b_hash)
{
  int i;

  if (0 != memcmp((uint32_t *)a_hash, 
        (uint32_t *)b_hash, sizeof(zdb_hash_t)))
    return (FALSE);

  return (TRUE);
}


/**
 * The base path of the blockchain database.
 */
const char *zdb_path_base(void)
{
  static char ret_path[PATH_MAX+1];

  if (!*ret_path) {
    struct stat st;
    const char *base_path;

    /* base hierarchial path */
    base_path = zsys_data_path();
    if (0 != stat(base_path, &st))
      mkdir(base_path, 0777);

    /* zdb module */
    sprintf(ret_path, "%szdb", base_path);
    if (0 != stat(ret_path, &st))
      mkdir(ret_path, 0700);
  }

  return ((const char *)ret_path);
}

void zdb_idle(zdb_t *db)
{
  zdb_map_t *map;
	time_t now;
  int i;

  if (!zdb_trylock(db))
		return; /* busy */

	now = time(NULL);

	map = NULL;
	for (i = 0; i < db->data_map_len; i++) {
		map = db->data_map + i;
		if (map->fd != 0) {
			if ((map->stamp + BCMAP_IDLE_TIME) <= now)
				break; /* one at a time */
		}

		map = NULL;
	}
	zdb_unlock(db);
	if (!map)
		return;

	if (zdb_trylock(db)) {
		zdb_map_close(map);
		zdb_unlock(db);
	}

}

uint32_t zdb_journal(int pos)
{
  return ( (pos / ZDB_BLOCKS_PER_JOURNAL) + 1 );
}

int zdb_clear(zdb_t *db, zdb_size_t pos)
{
  zdb_idx_t idx;
  int err;

  err = zdb_idx_get(db, pos, &idx);
  if (err)
    return (err);

  err = zdb_arch_add(db, &idx);
  if (err)
    return (err);

  err = zdb_idx_clear(db, pos);
  if (err)
    return (err);

  return (0);
}


static int _zdb_arch_write(zdb_t *db, zdb_hash_t hash, void *raw_data, zdb_size_t data_len)
{
  unsigned char *data = (unsigned char *)raw_data;
  zdb_size_t pos;
  zdb_idx_t idx;
  zdb_map_t *map;
  char ext[64];
  int jrnl;
  int err;

  /* use last journal */
	err = zdb_idx_next(db, &pos);
	if (err)
		return (err);

  jrnl = zdb_journal(pos);
  err = zdb_alloc(db, jrnl);
  if (err)
    return (err);

  map = db->data_map + jrnl;
  if (!*map->ext)
    sprintf(map->ext, "%u", idx.jrnl);

  /* finialize block index */
  memset(&idx, 0, sizeof(idx));
  idx.jrnl = jrnl;
  idx.size = data_len;
  idx.of = map->hdr->of;
  idx.crc = (crc64(0, data, data_len) & 0xFFFFFFFF);
  memcpy(idx.hash, hash, sizeof(zdb_hash_t));

  /* store serialized block data */
  err = zdb_map_append(db, map, data, data_len);
  if (err < 0)
    return (err);

  /* store archived block index */
  err = zdb_arch_add(db, &idx);
  if (err < 0)
    return (err);

  return (0);
}

int zdb_arch_write(zdb_t *db, zdb_hash_t hash, void *raw_data, zdb_size_t data_len)
{
  int err;

  if (!zdb_lock(db))
    return (ERR_NOLCK);
  err = _zdb_arch_write(db, hash, raw_data, data_len);
  zdb_unlock(db);

  return (err);
}

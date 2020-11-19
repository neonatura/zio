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
#include <sys/mman.h>

#ifdef WINDOWS
#define ZDB_MAP_BLOCK_SIZE 65536
#else
#define ZDB_MAP_BLOCK_SIZE 16384
#endif

#define MAX_ZDB_MAP_SIZE 0xFFFFFFF0

static int _zdb_map_open(zdb_t *db, zdb_map_t *map)
{
  zdb_hdr_t ini_hdr;
  struct stat st;
  char path[PATH_MAX+1];
  zdb_size_t size;
  int err;
  int fd;

  if (map->fd != 0)
    return (0);

  sprintf(path, "%s/%s.%s", zdb_path_base(), zdb_name(db), map->ext);
#ifdef WINDOWS
  fd = open(path, O_RDWR | O_CREAT, 00777);
#else
  fd = open(path, O_RDWR | O_CREAT, 00700);
#endif
  if (fd == -1) {
//    perror("zdb_map_open [open]");
    return (zsyserror());
  }

  memset(&st, 0, sizeof(st));
  err = fstat(fd, &st);
  if (err) {
    //perror("zdb_map_open [fstat]");
    close(fd);
    return (zsyserror());
  }
  if (!S_ISREG(st.st_mode)) {
    //perror("zdb_map_open [!reg]");
    close(fd);
    return (ERR_ISDIR);
  }
  
  memset(&ini_hdr, 0, sizeof(ini_hdr));
  err = read(fd, &ini_hdr, sizeof(ini_hdr));
  if (err != sizeof(ini_hdr) ||
      ini_hdr.magic != ZDB_MAGIC) {
    lseek(fd, 0L, SEEK_SET);

    st.st_size = ZDB_MAP_BLOCK_SIZE;
    err = ftruncate(fd, st.st_size);
    if (err) {
      //perror("zdb_map_open [truncate]");
      close(fd);
      return (zsyserror());
    }

    lseek(fd, 0L, SEEK_SET);
    memset(&ini_hdr, 0, sizeof(ini_hdr));
    ini_hdr.magic = ZDB_MAGIC;
    ini_hdr.stamp = ztime();
    write(fd, &ini_hdr, sizeof(ini_hdr));
  } 

  map->fd = fd;
  map->size = st.st_size;
  map->hdr = NULL;
  map->raw = NULL;

  return (0);
}

int zdb_map_open(zdb_t *db, zdb_map_t *map)
{
  int err;

  if (!zdb_lock(db))
    return (ERR_NOLCK);

  err = _zdb_map_open(db, map);
  zdb_unlock(db);

  return (err);
}

/**
 * @param The amount to incrase the allocated size by.
 */
static int _zdb_map_alloc(zdb_t *db, zdb_map_t *map, zdb_size_t len)
{
  struct stat st;
  unsigned char *raw;
  zdb_size_t size;
  zdb_size_t map_alloc;
  zdb_size_t map_of;
  char errbuf[256];
  int err;

  /* ensure file map is open */
  err = zdb_map_open(db, map);
  if (err) {
    sprintf(errbuf, "zdb_map_alloc: zdb_map_open error.");
    zlog_error(err, errbuf);
    return (err);
  }

  memset(&st, 0, sizeof(st));
  err = fstat(map->fd, &st);
  if (err) {
		err = zsyserror();
    sprintf(errbuf, "zdb_map_alloc: fstat [map fd %d]", map->fd);
    zlog_error(err, errbuf);
    return (err);
  }

  map_of = 0;
  size = st.st_size / ZDB_MAP_BLOCK_SIZE * ZDB_MAP_BLOCK_SIZE;
	if (size >= MAX_ZDB_MAP_SIZE)
		return (ERR_OVERFLOW);

  if (!map->hdr) { /* map has not been allocated */
    zdb_hdr_t hdr;

    memset(&hdr, 0, sizeof(hdr));
    lseek(map->fd, 0L, SEEK_SET);
    (void)read(map->fd, &hdr, sizeof(zdb_hdr_t));
    map_of = hdr.of;
  } else {
    /* map has already been allocated */
    map_of = map->hdr->of;

    if (sizeof(zdb_hdr_t) + map_of + len < st.st_size) {
      /* map is large enough */
      return (0);
    }
    zdb_map_free(map);
  }

  if (size < ZDB_MAP_BLOCK_SIZE ||
      sizeof(zdb_hdr_t) + map_of + len >= st.st_size) {
    /* enlarge map size */
    size = st.st_size + len;
    size = size + (size / 10); /* + 10% */
    size = ((size / ZDB_MAP_BLOCK_SIZE) + 1) * ZDB_MAP_BLOCK_SIZE;
    err = ftruncate(map->fd, size);
    if (err) {
			err = zsyserror();
      sprintf(errbuf, "zdb_map_alloc: ftruncate(fd %d, <%d bytes>)", map->fd, size);
      zlog_error(err, errbuf);
      return (err);
    }
  }

#ifdef WINDOWS
  fsync(map->fd); /* quash cache */
#endif

  /* map disk to memory */
  raw = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, map->fd, 0); 
  if (raw == MAP_FAILED) {
    sprintf(errbuf, "zdb_map_alloc: error: mmap failed on fd %d\n", map->fd); 
    zlog_error(ERR_NOMEM, errbuf);
    return (ERR_NOMEM); 
  }

  /* fill in file map structure */
  map->hdr = (zdb_hdr_t *)raw;
  map->raw = (raw + sizeof(zdb_hdr_t));
  map->size = size;

  return (0);
}

int zdb_map_alloc(zdb_t *db, zdb_map_t *map, zdb_size_t len)
{
  int err;

  if (!zdb_lock(db))
    return (ERR_NOLCK);

  err = _zdb_map_alloc(db, map, len);
  zdb_unlock(db);

  return (err);
}

/**
 * Truncates the data-content layer of a map to a given size.
 */
static int _zdb_map_trunc(zdb_t *db, zdb_map_t *map, zdb_size_t len)
{
  int err;

#if 0
  /* ensure file map is open */
  err = zdb_map_open(db, map);
  if (err)
    return (err);
#endif

  err = zdb_map_alloc(db, map, 0);
  if (err) {
    return (err);
  }

  if (len < map->hdr->of) {
    map->hdr->of = len;
  }

  return (0);
}

int zdb_map_trunc(zdb_t *db, zdb_map_t *map, zdb_size_t len)
{
  int err;

  if (!zdb_lock(db))
    return (ERR_NOLCK);

  err = _zdb_map_trunc(db, map, len);
  zdb_unlock(db);

  return (err);
}

void zdb_map_free(zdb_map_t *map)
{

  if (map->hdr) {
		munmap((void *)map->hdr, map->size); 

		map->hdr = NULL;
		map->raw = NULL;
		map->size = 0;
  }

}

static void _zdb_map_close(zdb_map_t *map)
{

  zdb_map_free(map);

  if (map->fd) {
    close(map->fd);
    map->fd = 0;
  }

}

void zdb_map_close(zdb_map_t *map)
{

  if (map) {
		_zdb_map_close(map);
  }

}

static int _zdb_map_write(zdb_t *db, zdb_map_t *map, zdb_size_t of, void *raw_data, zdb_size_t data_len)
{
  unsigned char *data = (unsigned char *)raw_data;
  int err;

  err = zdb_map_alloc(db, map, data_len);
  if (err)
    return (err);

  memcpy(map->raw + of, data, data_len);
  map->hdr->of = MAX(map->hdr->of, (of + data_len));
  map->stamp = time(NULL);

  return (0);
}

int zdb_map_write(zdb_t *db, zdb_map_t *map, zdb_size_t of, void *raw_data, zdb_size_t data_len)
{
  int err;

  if (!zdb_lock(db))
    return (ERR_NOLCK);
  err = _zdb_map_write(db, map, of, raw_data, data_len);
  zdb_unlock(db);

  return (err);
}

/**
 * Write some data to a specific filemap. 
 */
static int _zdb_map_append(zdb_t *db, zdb_map_t *map, void *raw_data, zdb_size_t data_len)
{
  unsigned char *data = (unsigned char *)raw_data;
  int err;

  err = zdb_map_alloc(db, map, data_len);
  if (err)
    return (err);

  return (zdb_map_write(db, map, map->hdr->of, data, data_len));
}

int zdb_map_append(zdb_t *db, zdb_map_t *map, void *raw_data, zdb_size_t data_len)
{
  int err;

  if (!zdb_lock(db))
    return (ERR_NOLCK);
  err = _zdb_map_append(db, map, raw_data, data_len);
  zdb_unlock(db);

  return (err);
}

static int _zdb_map_read(zdb_t *db, zdb_map_t *map, unsigned char *data, zdb_size_t data_of, zdb_size_t data_len)
{

  if ((data_of + data_len) >= map->hdr->of)
    return (ERR_INVAL);

  memcpy(data, map->raw + data_of, data_len);
  map->stamp = time(NULL);

  return (0);
}

/**
 * Read a segment data from a file-map
 */
int zdb_map_read(zdb_t *db, zdb_map_t *map, unsigned char *data, zdb_size_t data_of, zdb_size_t data_len)
{
  int err;

  if (!zdb_lock(db))
		return (ERR_NOLCK);

	err = _zdb_map_read(db, map, data, data_of, data_len);
	zdb_unlock(db);
	if (err)
		return (err);

  return (0);
}

unsigned int zdb_fmap_total(zdb_t *db)
{
  zdb_map_t *map;
  int total;
  int i;

  total = 0;
  for (i = 0; i < db->data_map_len; i++) {
    map = db->data_map + i;
    if (map->fd != 0)
      total++;
  }

  return (total);
}

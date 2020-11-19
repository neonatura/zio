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

#ifndef __ZDB__ZDB_H__
#define __ZDB__ZDB_H__

#ifdef USE_LIBPTHREAD
#include <pthread.h>
typedef pthread_mutex_t bclock_t;
#else
typedef int bclock_t;
#endif

#include "zsys.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef linux
#define getpagesize() sysconf(_SC_PAGESIZE)
#endif

#define ZDB_BLOCKS_PER_JOURNAL 65536

#define ZDB_MAX_NAME_LENGTH 136

#define ZDB_MAGIC 0x22888822

#define BCMAP_LOCK "bcmap_lock"

typedef uint32_t zdb_size_t;

typedef uint32_t bcpos_t;

typedef uint32_t zdb_hash_t[8];

typedef struct zdb_idx_t
{
  uint32_t jrnl;
  zdb_size_t of;
  zdb_size_t size;
  uint32_t crc;
  uint64_t __reserved_0__;
  uint64_t __reserved_1__;
  zdb_hash_t hash;
} zdb_idx_t; /* 16b */

typedef struct zdb_hdr_t
{
  /** The last time the map was accessed */
  ztime_t stamp;
  /** An arbritrary number verifying this is a file-map. */
  uint32_t magic;
  /** The offset of data written by the user. */
  zdb_size_t of; 
} zdb_hdr_t; /* 16b */

typedef struct zdb_map_t
{
  volatile int fd;
  volatile size_t size;
  time_t stamp; 
  char ext[64];
  zdb_hdr_t *hdr;
  uint8_t *raw;
} zdb_map_t;

typedef struct zdb_t
{
  char name[ZDB_MAX_NAME_LENGTH];
  zdb_map_t idx_map;
  zdb_map_t tab_map;
  zdb_map_t arch_map;
  zdb_map_t *data_map;
  size_t data_map_len;
	bclock_t lk;
} zdb_t;

#include "zdb_mutex.h"
#include "zdb_fmap.h"
#include "zdb_index.h"
#include "zdb_table.h"
#include "zdb_arch.h"
#include "zdb_block.h"

#ifdef __cplusplus
}
#endif

#endif /* ndef __ZDB__ZDB_H__ */

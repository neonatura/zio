/*
 *  Copyright 2020 Brian Burrell
 *
 *  This file is part of zio.
 *  (https://github.com/neonatura/zio)
 *
 *  zio is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  zio is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with zio.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __HTM_MAP_H__
#define __HTM_MAP_H__

#define INITIAL_MAX 15 /* tunable == 2^n - 1 */

typedef uint64_t hkey_t;

typedef struct hmap_entry_t
{
  hkey_t key;
  unsigned int hash;
  void *val;
  struct hmap_entry_t *next;
} hmap_entry_t;

struct hmap_t;
typedef struct hmap_index_t
{
  struct hmap_t *ht;
  hmap_entry_t *tthis, *next;
  unsigned int index;
} hmap_index_t;

typedef struct hmap_t
{
  hmap_entry_t **array;
  hmap_index_t iterator;  /* For hmap_first(...) */
  unsigned int count, max;
  hmap_entry_t *free;  /* List of recycled entries */
} hmap_t;

hmap_t *hmap_init(void);

void hmap_free(hmap_t **meta_p);

int hmap_set(hmap_t *ht, hkey_t key, void *val);

void *hmap_get(hmap_t *ht, hkey_t key);

#endif /* ndef __HTM_MAP_H__ */

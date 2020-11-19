
/*
 *  Copyright 2020 Brian Burrell
 *
 *  This file is part of libhtm.
 *  (https://github.com/neonatura/zio)
 *
 *  libhtm is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Shionbot is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with libhtm.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "zsys.h"

hmap_index_t *hmap_next(hmap_index_t *hi)
{
	hi->tthis = hi->next;
	while (!hi->tthis) {
		if (hi->index > hi->ht->max)
			return NULL;

		hi->tthis = hi->ht->array[hi->index++];
	}
	hi->next = hi->tthis->next;
	return hi;
}

hmap_index_t *hmap_first(hmap_t *ht)
{
  hmap_index_t *hi;

  if (!ht)
    return (NULL);

  hi = &ht->iterator;

  hi->ht = ht;
  hi->index = 0;
  hi->tthis = NULL;
  hi->next = NULL;

  return hmap_next(hi);
}

static hmap_entry_t **alloc_array(hmap_t *ht, size_t max)
{
	hmap_entry_t **ret_ar;

	ret_ar = (hmap_entry_t **)calloc(max+1, sizeof(hmap_entry_t *));
	if (!ret_ar)
		return (NULL);

	return (ret_ar);
}

static void _expand_array(hmap_t *ht)
{
	hmap_index_t *hi;
	hmap_entry_t **new_array;
	unsigned int new_max;

	new_max = ht->max * 2;
	new_array = alloc_array(ht, new_max);
	for (hi = hmap_first(ht); hi; hi = hmap_next(hi)) {
		unsigned int i = hi->tthis->hash & new_max;
		hi->tthis->next = new_array[i];
		new_array[i] = hi->tthis;
	}
	if (ht->array)
		free(ht->array);
	ht->array = new_array;
	ht->max = new_max;
}

static unsigned int hmap_hash_num(unsigned char *key)
{
  ssize_t klen = sizeof(hkey_t);
  unsigned int hash = 0;
  const unsigned char *p;
  ssize_t i;

  for (p = key, i = klen; i; i--, p++) {
    hash = hash * 33 + *p;
  }

  return hash;
}

static hmap_entry_t **find_entry(hmap_t *ht, hkey_t *key, const void *val)
{
	ssize_t klen = sizeof(hkey_t);
	hmap_entry_t **hep, *he;
	unsigned int hash;

	if (!key)
		return (NULL);

	hash = hmap_hash_num((unsigned char *)key);

	/* scan linked list */
	for (hep = &ht->array[hash & ht->max], he = *hep;
			he; hep = &he->next, he = *hep) {
		if (he->hash == hash
				//      && he->klen == klen
				&& memcmp(&he->key, key, sizeof(hkey_t)) == 0)
			break;
	}
	if (he || !val)
		return hep;

	/* add a new entry for non-NULL values */
	if ((he = ht->free) != NULL)
		ht->free = he->next;
	else
		he = (hmap_entry_t *)calloc(1, sizeof(hmap_entry_t));
	he->next = NULL;
	he->hash = hash;
	memcpy(&he->key, key, sizeof(hkey_t));
//	he->key  = hkey_clone(key);
	//  he->klen = klen;

//	he->flag = 0;
//	he->sz = 0;
	he->val  = (void *)val;
	*hep = he;
	ht->count++;

	return (hep);
}

void *hmap_get(hmap_t *ht, hkey_t key)
{
	hmap_entry_t *he;

	if (!ht)
		return (NULL);

	he = *find_entry(ht, &key, NULL);
	if (he)
		return (he->val);

	return (NULL);
}

int hmap_set(hmap_t *ht, hkey_t key, void *val)
{
  const unsigned char *data = (unsigned char *)val;
  hmap_entry_t **hep;

  if (!ht || !key)
    return (-EINVAL);

  hep = find_entry(ht, &key, data);
  if (!hep)
    return (-EINVAL);

  if (*hep) {
    if (!data) {
      /* delete entry */
      hmap_entry_t *old = *hep;

      *hep = (*hep)->next;
      old->next = ht->free;
      ht->free = old;
      --ht->count;

			memset(&old->key, 0, sizeof(old->key));
      old->val = NULL;
    } else {
      hmap_entry_t *cur = *hep;

      /* replace entry */
      cur->val = val;

      /* check that the collision rate isn't too high */
      if (ht->count > ht->max) {
        _expand_array(ht);
      }
    }
  }

  /* else key not present and val==NULL */
  return (0);
}

hmap_t *hmap_init(void)
{
	hmap_t *ht;

	ht = (hmap_t *)calloc(1, sizeof(hmap_t));
	ht->free = NULL;
	ht->count = 0;
	ht->max = INITIAL_MAX;
	ht->array = alloc_array(ht, ht->max);

	return (ht);
}

void hmap_free(hmap_t **meta_p)
{
  hmap_entry_t *e_next;
  hmap_entry_t *ent;
  hmap_t *meta;
  int i;

  if (!meta_p)
    return;

  meta = *meta_p;
  *meta_p = NULL;
  if (!meta)
    return;

  for (i = 0; i <= meta->max; i++) {
    for (ent = meta->array[i]; ent; ent = e_next) {
      e_next = ent->next;

#if 0
      if ((ent->flag & SHMAP_ALLOC) && ent->val)
        free((void *)ent->val);

      hkey_free(&ent->key);
#endif

      free(ent);
    }
  }

  /* recycle bucket */
  for (ent = meta->free; ent; ent = e_next) {
    e_next = ent->next;

//    hkey_free(&ent->key);
    free(ent);
  }

	free(meta->array);
	free(meta);
}


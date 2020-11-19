
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

#include "htm.h"

void htm_cache_set(cache_t *cache, uint64_t csum, chord_t *hash)
{
  int i;
	int k;

  for (i = 0; i < CACHE_SIZE; i++) {
		if (cache[i].key == csum || /* already cached. */
				i == (CACHE_SIZE - 1)) {
			if (i == 0) {
				memcpy(&cache[i].seq, hash, sizeof(chord_t));
			} else {
				for (k = i; k < CACHE_SIZE; k++) {
					if (cache[k].key != csum)
						memcpy(&cache[k], &cache[k-1], sizeof(cache_t));
					else
						memset(&cache[k], 0, sizeof(cache_t));
				}
				cache[i-1].key = csum;
				memcpy(&cache[i-1].seq, hash, sizeof(chord_t));
			}
      return;
    }
  }

}

void htm_cache_sethash(cache_t *cache, chord_t *key, chord_t *hash)
{
  uint64_t csum;
  int i;

  csum = htm_chord_compact(key);
  if (csum == 0)
    return;

	return (htm_cache_set(cache, csum, hash));
}

static double htm_cache_match(chord_t *in_hash, chord_t *c_hash)
{
	double total;
	double count;
	int idx;
	int bit;

	total = 0;
	count = 0;
	for (idx = 0; idx < CHORD_SIZE; idx++) {
		for (bit = 0; bit < 8; bit++) {
			if (c_hash->vec[idx] & (1 << bit)) {
				if (in_hash->vec[idx] & (1 << bit))
					count += 1;
				total += 1;
			}
		}
	}
	if (total == 0)
		return (0);

	return (100 / total * count);
}

chord_t *htm_cache_get(cache_t *cache, chord_t *in_hash)
{
	chord_t hash;
	int col;
	int idx;

	if (!cache || !in_hash)
		return (FALSE);

	memset(&hash, 0, CHORD_SIZE);
	for (idx = 0; idx < CACHE_SIZE; idx++) {
		if (cache[idx].key == 0)
			continue;

		memset(&hash, 0, CHORD_SIZE);
		htm_chord_expand(&hash, cache[idx].key);
		if (htm_cache_match(in_hash, &hash))
			return (&cache[idx].seq);
	}

	return (NULL);
}


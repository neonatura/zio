/*
 *  Copyright 2020 Neo Natura
 *
 *  This file is part of zio project.
 *  (https://github.com/neonatura/zio)
 *
 *  zio project is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  zio project is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with zio project.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "htm.h"
#include "assert.h"

	cell_t *cold_cell;
	cell_t *warm_cell;
void htm_sense_brane_init(entity_t *ent, brane_t *br)
{
	tree_t *cache;
	cell_t *int_cell;
	cell_t *cell;
	pos_t pos;
	uint64_t key;
	path_t *path;
	double ar[3];
	chord_t hash;
	int x, y;

  cache = htm_brane_cache(br);

	/* internal "position" */
	memset(&pos, 0, sizeof(pos));
	htm_position_gen(&pos);
	key = htm_chord_compact(&pos.loc_hash);
	int_cell = htm_cell_init(br);
	htm_tree_cache_set(cache, key, int_cell); 

	/* sense when temperature is cold. */
	ar[0] = 0.1;
	ar[1] = ar[2] = 0;
	memset(&hash, 0, sizeof(hash));
	htm_chord_split_set(&hash, 3, ar, MAX_CHORD_SPLIT_SIZE);
	key = htm_chord_compact(&hash);
	cold_cell = htm_cell_init(br);
	htm_tree_cache_set(cache, key, cold_cell); 
	(void)htm_path_new(br, int_cell, cold_cell);

	/* sense when temperature is warm. */
	ar[0] = 0.9;
	ar[1] = ar[2] = 0;
	memset(&hash, 0, sizeof(hash));
	htm_chord_split_set(&hash, 3, ar, MAX_CHORD_SPLIT_SIZE);
	key = htm_chord_compact(&hash);
	warm_cell = htm_cell_init(br);
	htm_tree_cache_set(cache, key, warm_cell); 
	(void)htm_path_new(br, int_cell, warm_cell);

	/* external positions. */
	for (x = 0; x < 32; x++) {
		for (y = 0; y < 32; y++) {
			memset(&pos, 0, sizeof(pos));
			pos.loc_x = x;
			pos.loc_y = y;
			htm_position_gen(&pos);
			key = htm_chord_compact(&pos.loc_hash);
			cell = htm_cell_init(br);
			assert(cell);
			htm_tree_cache_set(cache, key, cell); 

			/* internalize the external stimuli. */
			path = htm_path_new(br, cell, int_cell);
		}
	}

/* 
 * .. next, two [or more] cells which are biased to only react to extreme temperatures, pressures, etc. 
 */
}

chord_t *htm_sense_hash(double temp, double pressure, double prop)
{
  static chord_t ret_hash;
  double ar[3];

  /* calculate a 0 to 1 range value for a temperature range. */
  temp = MIN(MAX_SENSE_TEMP - 1, temp);
  temp = MAX(0, temp - MIN_SENSE_TEMP);
  temp = temp / (MAX_SENSE_TEMP - MIN_SENSE_TEMP);
  ar[0] = temp;

  /* calcaulte a 0 to 1 range value for applied pressure. */
  pressure = MIN(MAX_SENSE_PRESSURE - 1, pressure);
  pressure = MAX(0, pressure - MIN_SENSE_PRESSURE);
  pressure = pressure / (MAX_SENSE_PRESSURE - MIN_SENSE_PRESSURE);
  ar[1] = pressure;

  /* stretch is calculated as 0 to 1 representing percentage. */
  ar[2] = prop;

  memset(&ret_hash, 0, sizeof(ret_hash));
  htm_chord_split_set(&ret_hash, 3, (double *)ar, MAX_CHORD_SPLIT_SIZE);

  return ((chord_t *)&ret_hash);
}

void htm_sense_notify_internal(entity_t *ent, chord_t *hash)
{
	brane_t *br = htm_brane_get(ent, BR_SENSE);

fprintf(stderr, "DEBUG: htm_sense_notify_internal: hash \"%s\"\n", htm_chord_hex(hash));
	htm_cell_notify(ent, br, cold_cell, hash);
	htm_cell_notify(ent, br, warm_cell, hash);
}


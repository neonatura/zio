
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

int htm_layer_verify(layer_t *l, chord_t *hash)
{
	return (htm_chord_match(&l->hash, hash));
}

#if 0
int htm_layer_predict(layer_t *l, chord_t *in_hash)
{
	chord_t hash;
	int col;
	int idx;

	if (!l->cache || !in_hash)
		return (FALSE);

	memset(&hash, 0, CHORD_SIZE);
	for (idx = 0; idx < LAYER_CACHE_SIZE; idx++) {
		if (l->cache[idx] == 0) 
			continue;

		memset(&hash, 0, CHORD_SIZE);
		htm_chord_expand(&hash, l->cache[idx]);
		if (!htm_chord_match(&l->hash, &hash) &&
				htm_chord_match(in_hash, &hash)) {
			htm_chord_pool(&l->hash, &hash);
			break;
		}
	}
	if (idx != LAYER_CACHE_SIZE)
		return (TRUE);

	htm_layer_cache(l, in_hash); 
	return (FALSE);
}
#endif

int htm_layer_action(layer_t *l)
{
	int spike;
	int idx;

	spike = FALSE;
	for (idx = 0; idx < CHORD_SIZE; idx++) {
		if (l->level[idx] >= HTM_ACTION_THRESHOLD) {
			spike = TRUE;
			l->level[idx] = HTM_ACTION_REST;
		}
	}

	return (spike);
}

int htm_layer_set(layer_t *l, chord_t *hash)
{
	int spike;

#if 0
	/* reduce weight over time. */
	htm_chord_weight_apply(&l->hash);
	switch (op) {
		case OP_RESET:
			/* mark entire columns for prediction. */
			htm_chord_reset(&l->hash, hash);
			break;

		case OP_SEQ:
			/* sequential over-write with activate bias based on past column weight. */
			memset((uint8_t *)l->hash.vec, 0, CHORD_SIZE);
			htm_chord_set(&l->hash, hash);
			break;

		case OP_POOL:
			/* augment layer */
			htm_chord_pool(&l->hash, hash);
			break;

		case OP_FEEDBACK:
			/* depolarize layer */
			htm_chord_feedback(&l->hash, hash);
			break;
	}
#endif

  if (l->op) {
    (*l->op)(l, hash, 0);
  } else {
		fprintf(stderr, "DEBUG: layer{%x} has no op\n", l);
	}

	spike = htm_layer_action(l);
	if (spike)
		htm_layer_inhibit(l);

	return (spike);
}

#if 0
void htm_layer_pool(layer_t *l, chord_t *hash)
{
	htm_chord_pool(&l->hash, hash);
}
#endif

void htm_layer_level_incr(layer_t *hash, int idx)
{
  hash->level[idx] = MIN(127, (int)hash->level[idx] + 5);
}

void htm_layer_level_decr(layer_t *l, int idx)
{
	l->level[idx] = MAX(-127, (int)l->level[idx] - 5);
}

void htm_layer_excite(layer_t *l)
{
  int idx;

  for (idx = 0; idx < CHORD_SIZE; idx++) {
		htm_layer_level_incr(l, idx);
  }
}

void htm_layer_inhibit(layer_t *l)
{
  int idx;

  for (idx = 0; idx < CHORD_SIZE; idx++) {
		htm_layer_level_decr(l, idx);
  }
}




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

/**
 * @returns -1 (negative) to 1 (positive)
 */
double htm_emo_cell_weight(entity_t *ent, cell_t *cell)
{
	chord_t *h_obj;
	double val[2];

	h_obj = htm_cell_object(cell);
  htm_chord_split_get(h_obj, 2, val, MAX_CHORD_SPLIT_SIZE);
	return (val[0] - val[1]); /* positive sans negative */
}

void htm_emo_cell_dampen(entity_t *ent, cell_t *cell)
{
	chord_t *h_obj;
	double val[2];

	h_obj = htm_cell_object(cell);
  htm_chord_split_get(h_obj, 2, val, MAX_CHORD_SPLIT_SIZE);
	val[0] = MAX(0.0000, val[0] - (val[0] / 100));
	val[1] = MAX(0.0000, val[1] - (val[1] / 100));
	memset(h_obj->vec, 0, CHORD_SIZE);
  htm_chord_split_set(h_obj, 2, val, MAX_CHORD_SPLIT_SIZE);
}

double htm_emo_weight(entity_t *ent, chord_t *hash)
{
	brane_t *br;
	tree_t *cache;
	cell_t *cell;
	chord_t *h_obj;
	double val[2];
	uint64_t key;
	
	br = htm_brane_get(ent, BR_EMOTION);
	if (!br)
		return (0.0);

	cache = htm_brane_cache(br);
	if (!cache)
		return (0.0);

	key = htm_chord_compact(hash);
	cell = htm_tree_cache_get(cache, key);
	if (!cell)
		return (0.0);

	h_obj = htm_cell_object(cell);
	htm_chord_split_get(h_obj, 2, val, MAX_CHORD_SPLIT_SIZE);
	return (val[0] - val[1]); /* positive sans negative */
}

double htm_emo_resonance(entity_t *ent, chord_t *hash)
{
	brane_t *br;
	tree_t *cache;
	cell_t *cell;
	chord_t *h_obj;
	double val[2];
	uint64_t key;
	
	br = htm_brane_get(ent, BR_EMOTION);
	if (!br)
		return (0.0);

	cache = htm_brane_cache(br);
	if (!cache)
		return (0.0);

	key = htm_chord_compact(hash);
	cell = htm_tree_cache_get(cache, key);
	if (!cell)
		return (0.0);

	h_obj = htm_cell_object(cell);
	htm_chord_split_get(h_obj, 2, val, MAX_CHORD_SPLIT_SIZE);
	return ((val[0] + val[1]) / 2); /* sum of positive and negative */
}

void htm_emo_weight_set(entity_t *ent, chord_t *hash, double nval)
{
	brane_t *br;
	tree_t *cache;
	cell_t *cell;
	chord_t *h_obj;
	double val[2];
	uint64_t key;
	
	br = htm_brane_get(ent, BR_EMOTION);
	if (!br)
		return;

	cache = htm_brane_cache(br);
	if (!cache)
		return;

	key = htm_chord_compact(hash);
	cell = htm_tree_cache_get(cache, key);
	if (!cell) {
		cell = htm_cell_init(br);
		if (!cell)
			return;
		htm_tree_cache_set(cache, key, cell);
	}

	h_obj = htm_cell_object(cell);
	htm_chord_split_get(h_obj, 2, val, MAX_CHORD_SPLIT_SIZE);
	if (nval > 0.0000)
		val[0] = (nval + val[1]);
	else if (nval < 0.0000)
		val[1] = (nval + val[0]);
	else 
		val[0] = val[1] = MIN(val[0], val[1]);
	memset(h_obj->vec, 0, CHORD_SIZE);
	htm_chord_split_set(h_obj, 2, val, MAX_CHORD_SPLIT_SIZE);
}

void htm_emo_weight_incr(entity_t *ent, chord_t *hash)
{
	brane_t *br;
	tree_t *cache;
	cell_t *cell;
	chord_t *h_obj;
	double val[2];
	uint64_t key;
	
	br = htm_brane_get(ent, BR_EMOTION);
	if (!br)
		return;

	cache = htm_brane_cache(br);
	if (!cache)
		return;

	key = htm_chord_compact(hash);
	cell = htm_tree_cache_get(cache, key);
	if (!cell)
		return;

	h_obj = htm_cell_object(cell);
	htm_chord_split_get(h_obj, 2, val, MAX_CHORD_SPLIT_SIZE);
	val[0] += MIN(1.0, val[0] / 100);
	memset(h_obj->vec, 0, CHORD_SIZE);
	htm_chord_split_set(h_obj, 2, val, MAX_CHORD_SPLIT_SIZE);
}

void htm_emo_weight_decr(entity_t *ent, chord_t *hash)
{
	brane_t *br;
	tree_t *cache;
	cell_t *cell;
	chord_t *h_obj;
	double val[2];
	uint64_t key;
	
	br = htm_brane_get(ent, BR_EMOTION);
	if (!br)
		return;

	cache = htm_brane_cache(br);
	if (!cache)
		return;

	key = htm_chord_compact(hash);
	cell = htm_tree_cache_get(cache, key);
	if (!cell)
		return;

	h_obj = htm_cell_object(cell);
	htm_chord_split_get(h_obj, 2, val, MAX_CHORD_SPLIT_SIZE);
	val[1] += MIN(1.0, val[1] / 100);
	memset(h_obj->vec, 0, CHORD_SIZE);
	htm_chord_split_set(h_obj, 2, val, MAX_CHORD_SPLIT_SIZE);
}

double htm_emo_mood(entity_t *ent)
{
	cell_t *pr_cell = ent->brane_cell[BR_EMOTION];

	return (htm_emo_cell_weight(ent, pr_cell));
}

void htm_emo_mood_incr(entity_t *ent)
{
	cell_t *pr_cell = ent->brane_cell[BR_EMOTION];

	htm_emo_weight_incr(ent, htm_cell_object(pr_cell));
}

void htm_emo_mood_decr(entity_t *ent)
{
	cell_t *pr_cell = ent->brane_cell[BR_EMOTION];

	htm_emo_weight_decr(ent, htm_cell_object(pr_cell));
}

void htm_emo_mood_dampen(entity_t *ent)
{
	cell_t *pr_cell = ent->brane_cell[BR_EMOTION];

	htm_emo_cell_dampen(ent, pr_cell);
}

void htm_emo_brane_init(entity_t *ent, brane_t *br)
{

}

void *htm_emo_brane_run(entity_t *ent)
{

	while (htm_control_state(ent) != STATE_DEAD) {

		/* continually dampen mood to a neutral state at degree based on control state. */
		htm_emo_mood_dampen(ent);

		htm_msleep(ent->control.rate);
	}

	return (NULL);
}


void htm_emo_brane_term(entity_t *ent, brane_t *br)
{

}


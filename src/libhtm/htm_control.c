
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

int htm_control_state(entity_t *ent)
{
	return (ent->control.state);
}

int htm_control_state_spike(entity_t *ent)
{
	double span;
	ztime_t now;

	now = ztime_now();
	span = (double)(now - ent->control.stamp);
	if (span >= ent->control.rate) {
		ent->control.stamp = now;
		return (TRUE);
	}
	
	return (FALSE);
}

void htm_control_state_set(entity_t *ent, int state)
{

	if (state < STATE_SLEEP ||
			state >= MAX_STATE_FOCUS)
		return;

	ent->control.state = state;
	ent->control.state_stamp = ztime_now();

	switch (state) {
		case STATE_DEAD:
			ent->control.pref_rate = 0;
			ent->control.saliency = 0;
			break;
		case STATE_SLEEP:
			ent->control.pref_rate = 5400;
			ent->control.saliency = 0.009;
			break;
		case STATE_REST: /* "falling asleep" */
			ent->control.pref_rate = 2700;
			ent->control.saliency = 0.018;
			break;
		case STATE_RECALL:
			/* "sleep spindles" */
			ent->control.pref_rate = 1;
			ent->control.saliency = 0.09;
			break;
		case STATE_IDLE:
			/* "restful wake" */
			ent->control.pref_rate = 0.25;
			ent->control.saliency = 0.9;
			break;
		case STATE_FOCUS_POSITION:
		case STATE_FOCUS_FEATURE:
		case STATE_FOCUS_OBJECT:
			/* "gamma" waves */
			ent->control.pref_rate = 0.025;
			ent->control.saliency = 0.99;
			break;
		case STATE_FOCUS_MEDITATE:
			ent->control.pref_rate = 0.025;
			ent->control.saliency = 0.01;
			break;
	}

}

void htm_control_state_incr(entity_t *ent)
{
	htm_control_state_set(ent, htm_control_state(ent) + 1);
}

void htm_control_state_decr(entity_t *ent)
{
	htm_control_state_set(ent, htm_control_state(ent) - 1);
}

double htm_control_laplace(double span, double seed, double rate)
{
	/* a unique sine wave with the frequency of <rate>. */
	return (zlaplace(span, seed, rate));
}

void htm_control_hash(entity_t *ent, chord_t *ret_hash)
{
	chord_t hash;
	double freq;
	double span;
	double life_span;
	double seed;
	double rate;
	uint64_t csum;
	uint32_t cseed;
	ztime_t now;
	int i;

	now = ztime_now();

  /* the total timespan spent in current state session. */
  span = (double)(now - ent->control.state_stamp) / 100;

  /* the total timespan of entity runtime. */
  life_span = (double)(now - ent->control.birth_stamp) / 100;

  /* a persistent unique identifier for the entity. */
	seed = (double)ent->id / (double)UINT64_MAX;

	/* the rate of activation [in ztime]. */ 
	rate = (double)ent->control.rate / (double)100;

	/* the sum of two sine waves representing the current focus session and the lifespan of the entity. */
	ent->control.wave = 
		htm_control_laplace(span, seed, rate) +
		htm_control_laplace(life_span, seed, 5400);

	ent->sess_freq = freq;
	csum = (uint64_t)((1 + ent->control.wave) * (double)(UINT_MAX/4));

	cseed = 0;
	memset(&hash, 0, sizeof(hash));
	htm_chord_digest(&hash, &cseed, (uint8_t *)&csum, sizeof(uint64_t));
	htm_chord_reduce(&hash, 1 / rate * 0.025);

fprintf(stderr, "DEBUG: TIMESTAMP HASH: %s\n", htm_chord_hex(&hash));

	memcpy(&ret_hash, &hash, sizeof(chord_t));
#if 0
	/* append a unique timestamp. */
	htm_chord_pool(ret_hash, &hash);
	/* append the current focused object. */
	htm_chord_pool(ret_hash, htm_focused_object(ent)); 
#endif

}

void htm_control_notify(entity_t *ent)
{
	int i;

	/* generate a timestamp for the session. */
	htm_control_hash(ent, &ent->control.state_hash);

	/* notify all branes. */
	for (i = 0; i < MAX_ENTITY_BRANES; i++) {
		if (!ent->brane[i] || !ent->brane[i]->def || 
				!ent->brane[i]->def->wake)
			continue;
		(*ent->brane[i]->def->wake)(ent, ent->brane[i]);
	}

}

chord_t *htm_state_hash(entity_t *ent)
{
	return (&ent->control.state_hash);
}

void htm_control_brane_init(entity_t *ent, brane_t *br)
{

//	ent->control.id = (uint32_t)(crc64(0, ent->name, strlen(ent->name)) & 0xFFFFFFFF);
	ent->control.birth_stamp = ztime_now();

}

void *htm_control_brane_run(entity_t *ent)
{

	htm_control_state_set(ent, STATE_IDLE);
	ent->control.rate = ent->control.pref_rate;

	while (htm_control_state(ent) != STATE_DEAD) {
		ent->control.rate = (ent->control.rate + ent->control.pref_rate) / 2;

		if (htm_control_state_spike(ent))
			htm_control_notify(ent);

//		htm_yield();
		zmsleep(ent->control.rate);
	}

	return (NULL);
}

void htm_control_brane_term(entity_t *ent, brane_t *br)
{
	ent->control.state = STATE_DEAD;
}

void htm_control_focus_position_set(entity_t *ent, chord_t *hash)
{
	brane_t *ctrl_br = htm_brane_get(ent, BR_CONTROL);

	htm_control_state_set(ent, STATE_FOCUS_POSITION);
	htm_cell_motor_apply(ent, ctrl_br, ent->brane_cell[BR_CONTROL], hash, NULL);
}

void htm_control_focus_feature_set(entity_t *ent, chord_t *hash)
{
	brane_t *ctrl_br = htm_brane_get(ent, BR_CONTROL);

	htm_control_state_set(ent, STATE_FOCUS_FEATURE);
	htm_cell_notify(ent, ctrl_br, ent->brane_cell[BR_CONTROL], hash);
}

/*
 * Focuses attention on sensory information pertaining to object which will be
 * appended to the episodic buffer.
 * @see htm_mem_set()
 */
void htm_control_focus_object_set(entity_t *ent, chord_t *hash)
{
	brane_t *ctrl_br = htm_brane_get(ent, BR_CONTROL);

	htm_control_state_set(ent, STATE_FOCUS_OBJECT);
	htm_cell_notify(ent, ctrl_br, ent->brane_cell[BR_CONTROL], hash);
}

chord_t *htm_focused_location(entity_t *ent)
{
	return (htm_cell_location(ent->brane_cell[BR_CONTROL]));
}

chord_t *htm_focused_object(entity_t *ent)
{
	return (htm_cell_object(ent->brane_cell[BR_CONTROL]));
}

chord_t *htm_focused_feature(entity_t *ent)
{
	return (htm_cell_feature(ent->brane_cell[BR_CONTROL]));
}

void htm_control_unfocus(entity_t *ent)
{
	htm_control_state_decr(ent);
	htm_cell_inhibit(ent->brane_cell[BR_CONTROL]);
}

#define MAX_CONTROL_CELL_TYPES 100
cell_t *htm_control_cell(entity_t *ent, int br_type, int subtype)
{
	brane_t *ctrl_br = htm_brane_get(ent, BR_CONTROL);
	tree_t *cache = htm_brane_cache(ctrl_br);
	cell_t *ret_cell;
	uint64_t key;

	key = ((uint64_t)br_type * (UINT64_MAX / MAX_ENTITY_BRANES)) + subtype;
	ret_cell = htm_tree_cache_get(cache, key);

	return (ret_cell);
}

void htm_control_reg_set(entity_t *ent, int reg, uint8_t val)
{
	cell_t *cell;
	chord_t *cell_hash;
	chord_t hash;
	layer_t *in_l;

	cell = htm_control_cell(ent, BR_CONTROL, BR_CONTROL_REGISTER);
	if (!cell)
		return;

	if (reg < 0 || reg >= CHORD_SIZE)
		return;

	in_l = &cell->layer[CELL_L4];
//	cell_hash = &in_l->hash; 

	memset(&hash, 0, sizeof(hash));
	hash.vec[reg] = val;
//	htm_chord_set(cell_hash, &hash); 
#if 0
	htm_layer_set(in_l, OP_SEQ, &hash); 
#endif
	htm_layer_set(in_l, &hash); 
}

uint8_t htm_control_reg_get(entity_t *ent, int subtype)
{
	cell_t *cell;
	chord_t *cell_hash;
	chord_t hash;
	layer_t *in_l;

	cell = htm_control_cell(ent, BR_CONTROL, BR_CONTROL_REGISTER);
	if (!cell)
		return (0);

	if (subtype < 0 || subtype >= CHORD_SIZE)
		return (0);

	in_l = &cell->layer[CELL_L4];
	cell_hash = &in_l->hash; 

	return (cell_hash->vec[subtype]);
}



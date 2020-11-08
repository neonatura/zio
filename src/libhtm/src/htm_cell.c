
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

static htm_op_f _op_table[MAX_CELL_LAYER] = {
	NULL,
	htm_op_pool, /* L1 */
	htm_op_reset, /* L2 */
	htm_op_pool, /* L3 */
	htm_op_seq, /* L4 */
	htm_op_pool, /* L5a */
	htm_op_seq, /* L5b */
	htm_op_seq, /* L6a */
	htm_op_seq, /* L6b */
};

cell_t *htm_cell_init(brane_t *br)
{
	cell_t *cell;
	uint64_t br_id;
	int idx;

	if (br->cell_len + 1 >= br->cell_max)
		return (NULL);

	cell = &br->cell[br->cell_len];
	memset(cell, 0, sizeof(cell_t));
	cell->cell_idx = br->cell_len;
	br_id = crc64(0, br->name, strlen(br->name));
	cell->id = crc64(br_id, &br->cell_len, sizeof(br->cell_len));
	br->cell_len++;

#if 0
	for (idx = 0; idx < MAX_CELL_LAYER; idx++) {
		memset((uint8_t *)cell->layer[idx].hash.vec, 0, CHORD_SIZE);
	}
#endif

	cell->brane_type = br->type;

	for (idx = 0; idx < MAX_CELL_LAYER; idx++) {
		cell->layer[idx].op = _op_table[idx];
	}

	/* cache hash reference to cell. */
	htm_brane_map_set(br, cell);

	return (cell);
}

int htm_cell_notify_exp(entity_t *ent, cell_t *cell)
{
	layer_t *exp_l = &cell->layer[CELL_L1];
	layer_t *obj_l = &cell->layer[CELL_L5a]; /* Object (pool) */
	layer_t *fe_l = &cell->layer[CELL_L3]; /* Feature (pool) */
	chord_t *c_hash;
	int spike;
	int ok;

#if 0
	spike = 0;
	spike |= htm_layer_set(exp_l, OP_POOL, &fe_l->hash);
	spike |= htm_layer_set(exp_l, OP_POOL, &obj_l->hash);
#endif
	spike = 0;
	spike |= htm_layer_set(exp_l, &fe_l->hash);
	spike |= htm_layer_set(exp_l, &obj_l->hash);

	c_hash = htm_cache_get(cell->cache, &exp_l->hash); 
	if (c_hash) {
#if 0
		htm_layer_set(fe_l, OP_POOL, c_hash);
		htm_layer_set(obj_l, OP_POOL, c_hash);
#endif
		htm_layer_set(fe_l, c_hash);
		htm_layer_set(obj_l, c_hash);
	} else {
		uint64_t csum;

		csum = htm_chord_compact(&obj_l->hash);
		htm_cache_set(cell->cache, csum, &exp_l->hash); 

		csum = htm_chord_compact(&fe_l->hash);
		htm_cache_set(cell->cache, csum, &exp_l->hash); 
	}

	if (!spike)
		return (FALSE);

	/*
	 * TODO:
	 * the thalamus sends (receive possible?) info to the L1 layer through
	 * m-type cells. these are thought to 'modulate' the info.
	 */
#if 0
	/* apply a unique timestamp. */
	htm_exec_stamp_hash(ent, &exp_l->hash);
#endif

fprintf(stderr, "DEBUG: EXP HASH: %s\n", htm_chord_hex(&exp_l->hash));

	return (TRUE);
}

#if 0
void htm_cell_notify_loc(cell_t *cell, chord_t *inset_hash)
{
	layer_t *seg_l = &cell->layer[CELL_L5b]; /* Feature@Location (seq) */
	layer_t *loc_l = &cell->layer[CELL_L6b]; /* Location (motor/grid) */
	layer_t *in_l = &cell->layer[CELL_L4]; /* Sensation@Orientation (Seq) */

	if (inset_hash)
		htm_layer_set(loc_l, OP_SEQ, inset_hash);

	/* set Location (L6b) */
	htm_layer_set(in_l, OP_FEEDBACK, &loc_l->hash);
}
#endif

#if 0
int htm_cell_notify_pos(cell_t *cell, chord_t *ori_hash)
{
	layer_t *in_l = &cell->layer[CELL_L4]; /* Sensation@Orientation (Seq) */
	layer_t *ori_l = &cell->layer[CELL_L6a]; /* Orientation (motor/axis) */

	if (!htm_layer_set(ori_l, OP_SEQ, ori_hash))
		return (FALSE); /* no output spike on layer */

	/* re-apply to L4 (feedback) */
  htm_chord_set(&in_l->hash, &ori_l->hash);
	return (TRUE);
}
#endif

#if 0
int htm_cell_notify_data(cell_t *cell, chord_t *in_hash)
{
	layer_t *in_l = &cell->layer[CELL_L4]; /* Sensation@Orientation (Seq) */

	/* Sensation -> L4 - apply stimuli data into input layer. */ 
	return (htm_layer_set(in_l, OP_SEQ, in_hash));
}
#endif

#if 0
int htm_cell_notify(entity_t *ent, cell_t *cell, uint64_t inset, uint64_t ori, uint8_t *data, size_t data_len)
{
	layer_t *exp_l = &cell->layer[CELL_L1];
	layer_t *prefe_l = &cell->layer[CELL_L2]; /* Feature (seq) */
	layer_t *fe_l = &cell->layer[CELL_L3]; /* Feature (pool) */
	layer_t *in_l = &cell->layer[CELL_L4]; /* Sensation@Orientation (Seq) */
	layer_t *obj_l = &cell->layer[CELL_L5a]; /* Object (pool) */
	layer_t *seg_l = &cell->layer[CELL_L5b]; /* Feature@Location (seq) */
	layer_t *ori_l = &cell->layer[CELL_L6a]; /* Orientation (motor/axis) */
	layer_t *loc_l = &cell->layer[CELL_L6b]; /* Location (motor/grid) */
	chord_t inset_hash;
	chord_t ori_hash;
	chord_t hash;
	uint64_t csum;
	int ok;

	csum = htm_chord_csum(data, data_len);
	memset(&hash, 0, CHORD_SIZE);
	htm_chord_expand(&hash, csum);
fprintf(stderr, "DEBUG: ORIGIN HASH: %s\n", htm_chord_hex(&hash));

	memset(&inset_hash, 0, CHORD_SIZE);
	htm_chord_expand(&inset_hash, inset);

	memset(&ori_hash, 0, CHORD_SIZE);
	htm_chord_expand(&ori_hash, ori);

	/* Sensation -> L4 - apply context data into input layer */
	if (!htm_layer_set(in_l, &hash))
		return (0); /* no output spike on layer */
fprintf(stderr, "DEBUG: INPUT HASH: %s\n", htm_chord_hex(&in_l->hash));

	/* Orientation <-> L6a */
	htm_layer_set(ori_l, &ori_hash);
	/* re-apply to L4 (feedback) */
  htm_chord_set(&in_l->hash, &ori_l->hash);

	/* apply Sense@Orientation (L4) -> Feature (L2) */
  htm_layer_set(prefe_l, &in_l->hash);
#if 0
	if (!htm_layer_spike(prefe_l))
		return (0);
#endif
	/* pool Feature (L2) -> Feature (L3) */
	htm_chord_hash_pool(&fe_l->hash, &prefe_l->hash); 
fprintf(stderr, "DEBUG: FEATURE HASH: %s\n", htm_chord_hex(&fe_l->hash));
#if 0
	/* pool Sense@Orientation (L4) -> Feature (L3) */
	htm_chord_hash_pool(&fe_l->hash, &in_l->hash); 
#endif
	if (!htm_layer_spike(fe_l))
		return (0);
#if 0
	/* pool Feature to Experience (L1) */
	htm_cell_notify_exp(cell, &fe_l->hash);
#endif

	/* Feature (L3) -> (L5b) Feature@Location */
	htm_layer_set(seg_l, &fe_l->hash);


#if 0
	/* set Location (L6b) */
	htm_chord_set(&loc_l->hash, &inset_hash);
	/* Location (L6b) -> Feature@Location (L5b) */
  htm_chord_set(&seg_l->hash, &loc_l->hash);
#endif
#if 0
	if (!htm_cell_notify_loc(cell, &inset_hash))
		return (0); /* no output spike on layer */
#endif
	htm_cell_notify_loc(cell, &inset_hash);

	/* pool Feature@Location (L5b) -> Object (L5a) */
	htm_layer_pool(obj_l, &seg_l->hash);
fprintf(stderr, "DEBUG: OBJECT HASH: %s\n", htm_chord_hex(&obj_l->hash));
	if (!htm_layer_spike(obj_l))
		return (0);
#if 0
	/* pool Object to Experience (L1) */
	htm_cell_notify_exp(cell, &obj_l->hash);
#endif

fprintf(stderr, "DEBUG: EXPERIENCE HASH: %s\n", htm_chord_hex(&exp_l->hash));

	/* pool Feature@Time to Experience (L1) */
	if (!htm_cell_notify_exp(ent, cell))
		return (0);

	htm_chord_inhibit(&fe_l->hash);
	htm_chord_inhibit(&obj_l->hash);
	htm_chord_inhibit(&exp_l->hash);

	/* remember */
	htm_mem_notify(ent, cell);

	return (0);
}
#endif

#if 0
void htm_cell_notify_ori(cell_t *cell, chord_t *ori_hash)
{
	layer_t *ori_l = &cell->layer[CELL_L6a]; /* Orientation (motor/axis) */
	layer_t *in_l = &cell->layer[CELL_L4]; /* Sensation@Orientation (Seq) */

	if (ori_hash) {
		/* Orientation <-> L6a */
		htm_layer_set(ori_l, OP_SEQ, ori_hash);
	}

	/* re-apply to L4 (feedback) */
	htm_layer_set(in_l, OP_FEEDBACK, &ori_l->hash);
}
#endif

void htm_cell_path_notify(entity_t *ent, brane_t *br, cell_t *cell)
{
	layer_t *obj_l = &cell->layer[CELL_L5a]; /* Object (pool) */
	layer_t *ori_l = &cell->layer[CELL_L6a]; /* Orientation (motor/axis) */
	layer_t *loc_l = &cell->layer[CELL_L6b]; /* Location (motor/grid) */
	cell_t *out_cell;
	chord_t ori_hash;
	chord_t loc_hash;
	path_t *p;
	int err;

	memcpy(&ori_hash, &ori_l->hash, sizeof(chord_t)); 
	memcpy(&loc_hash, &loc_l->hash, sizeof(chord_t)); 


	p = htm_path_first(br, cell);
	while (p) {
		out_cell = &br->cell[p->cell_idx];

		htm_cell_motor_apply(ent, br, out_cell, &loc_l->hash, &ori_l->hash);
		err = htm_cell_notify(ent, br, out_cell, &obj_l->hash);
		if (!err) {
#if 0
			/* project back location & orientation. */ 
			htm_chord_set(&ori_hash, &out_cell->layer[CELL_L6a].hash);
			htm_chord_set(&loc_hash, &out_cell->layer[CELL_L6b].hash);
#endif
		}

		p = htm_path_next(br, cell, p);
	}

//	htm_chord_set(&ori_l->hash, &ori_hash);
//	htm_chord_set(&loc_l->hash, &loc_hash);
}

#if 0
int htm_cell_notify_feature(cell_t *cell, chord_t *h_data)
{
	layer_t *prefe_l = &cell->layer[CELL_L2]; /* Feature (seq) */
	layer_t *fe_l = &cell->layer[CELL_L3]; /* Feature (pool) */
	int ok;

	/* pool Input -> Prefeature (L2) */
	ok = htm_layer_set(prefe_l, OP_SEQ, h_data);
fprintf(stderr, "DEBUG: PREFE_L: %s\n", htm_chord_hex(&prefe_l->hash));
	if (!ok)
		return (0);

	/* pool Prefeature (L2) -> Feature (L3) */
	htm_layer_set(fe_l, OP_POOL, &prefe_l->hash);
//	if (!ok) return (0);

	/* quash pool buildup */
//	htm_layer_set(prefe_l, OP_SEQ, h_data);

	return (1);
}
#endif

int htm_cell_notify(entity_t *ent, brane_t *br, cell_t *cell, chord_t *h_data)
{
	layer_t *exp_l = &cell->layer[CELL_L1];
	layer_t *prefe_l = &cell->layer[CELL_L2]; /* Feature (seq) */
	layer_t *fe_l = &cell->layer[CELL_L3]; /* Feature (pool) */
	layer_t *in_l = &cell->layer[CELL_L4]; /* Sensation@Orientation (Seq) */
	layer_t *obj_l = &cell->layer[CELL_L5a]; /* Object (pool) */
	layer_t *seg_l = &cell->layer[CELL_L5b]; /* Feature@Location (seq) */
	layer_t *ori_l = &cell->layer[CELL_L6a]; /* Orientation (motor/axis) */
	layer_t *loc_l = &cell->layer[CELL_L6b]; /* Location (motor/grid) */
	event_t *e;
	int ok;

	/* Sensation -> L4 - apply context data into input layer */
	//htm_cell_notify_data(cell, h_data);
	htm_layer_set(in_l, h_data);
//fprintf(stderr, "DEBUG: INPUT: %s\n", htm_chord_hex(&in_l->hash));

	/* Orientation -> L4 - modulates perspective */
//	htm_cell_notify_ori(cell, h_ori);

	/* apply Sense@Orientation (L4) -> Feature (L2) */
	ok = htm_layer_set(prefe_l, &in_l->hash);
//fprintf(stderr, "DEBUG: PREFE_L{%x}: %s\n", prefe_l, htm_chord_hex(&prefe_l->hash));
	if (!ok)
		return (0);

	/* pool Prefeature (L2) -> Feature (L3) */
	htm_layer_set(fe_l, &prefe_l->hash);
#if 0
	if (!htm_cell_notify_feature(cell, &in_l->hash))
		return (0);
#endif
//fprintf(stderr, "DEBUG: FEATURE HASH: %s\n", htm_chord_hex(&fe_l->hash));

	/* Feature (L3) -> (L5b) Feature@Location */
#if 0
	htm_layer_set(seg_l, OP_SEQ, &fe_l->hash);
#endif
	htm_layer_set(seg_l, &fe_l->hash);

	/* Location (L6b) -> Feature@Location (L5b) */
//	htm_cell_notify_loc(cell, h_inset);
#if 0
	htm_layer_set(seg_l, OP_POOL, &loc_l->hash);
#endif
	htm_layer_set(seg_l, &loc_l->hash);
//fprintf(stderr, "DEBUG: SEG HASH: %s\n", htm_chord_hex(&seg_l->hash));

	/* pool Feature@Location (L5b) -> Object (L5a) */
#if 0
	if (!htm_layer_set(obj_l, OP_POOL, &seg_l->hash))
		return (0);
#endif
	if (!htm_layer_set(obj_l, &seg_l->hash))
		return (0);
fprintf(stderr, "DEBUG: OBJECT HASH: %s\n", htm_chord_hex(&obj_l->hash));



#if 0
	csum = htm_chord_csum(data, data_len);
	memset(&hash, 0, CHORD_SIZE);
	htm_chord_expand(&hash, csum);
fprintf(stderr, "DEBUG: ORIGIN HASH: %s\n", htm_chord_hex(&hash));

	memset(&inset_hash, 0, CHORD_SIZE);
	htm_chord_expand(&inset_hash, inset);

	memset(&ori_hash, 0, CHORD_SIZE);
	htm_chord_expand(&ori_hash, ori);

	/* Sensation -> L4 - apply context data into input layer */
	if (!htm_layer_set(in_l, &hash))
		return (0); /* no output spike on layer */
fprintf(stderr, "DEBUG: INPUT HASH: %s\n", htm_chord_hex(&in_l->hash));

	/* Orientation <-> L6a */
	htm_layer_set(ori_l, &ori_hash);
	/* re-apply to L4 (feedback) */
  htm_chord_set(&in_l->hash, &ori_l->hash);

	/* apply Sense@Orientation (L4) -> Feature (L2) */
  htm_layer_set(prefe_l, &in_l->hash);
#if 0
	if (!htm_layer_spike(prefe_l))
		return (0);
#endif
	/* pool Feature (L2) -> Feature (L3) */
	htm_chord_hash_pool(&fe_l->hash, &prefe_l->hash); 
fprintf(stderr, "DEBUG: FEATURE HASH: %s\n", htm_chord_hex(&fe_l->hash));
#if 0
	/* pool Sense@Orientation (L4) -> Feature (L3) */
	htm_chord_hash_pool(&fe_l->hash, &in_l->hash); 
#endif
	if (!htm_layer_spike(fe_l))
		return (0);
#if 0
	/* pool Feature to Experience (L1) */
	htm_cell_notify_exp(cell, &fe_l->hash);
#endif

	/* Feature (L3) -> (L5b) Feature@Location */
	htm_layer_set(seg_l, &fe_l->hash);


#if 0
	/* set Location (L6b) */
	htm_chord_set(&loc_l->hash, &inset_hash);
	/* Location (L6b) -> Feature@Location (L5b) */
  htm_chord_set(&seg_l->hash, &loc_l->hash);
#endif
#if 0
	if (!htm_cell_notify_loc(cell, &inset_hash))
		return (0); /* no output spike on layer */
#endif
	htm_cell_notify_loc(cell, &inset_hash);

	/* pool Feature@Location (L5b) -> Object (L5a) */
	htm_layer_pool(obj_l, &seg_l->hash);
fprintf(stderr, "DEBUG: OBJECT HASH: %s\n", htm_chord_hex(&obj_l->hash));
	if (!htm_layer_spike(obj_l))
		return (0);
#if 0
	/* pool Object to Experience (L1) */
	htm_cell_notify_exp(cell, &obj_l->hash);
#endif

fprintf(stderr, "DEBUG: EXPERIENCE HASH: %s\n", htm_chord_hex(&exp_l->hash));
#endif





	/* pool Feature@Time to Experience (L1) */
	if (!htm_cell_notify_exp(ent, cell))
		return (0);

	/* linked cells */
	htm_cell_path_notify(ent, br, cell);

	/* feedback to brane module */
	if (br->def && br->def->read) {
		(*br->def->read)(ent, br, cell);
	}

	/* remember */
	e = htm_mem_set(ent, cell);

	/*
	htm_layer_inhibit(fe_l);
	htm_layer_inhibit(obj_l);
	*/

	return (0);
}

#if 0
int htm_cell_motor_apply(entity_t *ent, brane_t *br, cell_t *cell, chord_t *h_inset, chord_t *h_ori)
{
	layer_t *prefe_l = &cell->layer[CELL_L2]; /* Feature (seq) */
	layer_t *seg_l = &cell->layer[CELL_L5b]; /* Feature@Location (seq) */
	layer_t *ori_l = &cell->layer[CELL_L6a]; /* Orientation (motor/axis) */
	layer_t *loc_l = &cell->layer[CELL_L6b]; /* Location (motor/grid) */
	pos_t *pos;

	/* displacement of cell's physical location & orientation. */
	pos = htm_cell_position_translate(cell, h_inset, h_ori);

	/* Orientation -> L4 - modulates perspective */
//	htm_layer_set(ori_l, OP_SEQ, ori_hash);
#if 0
	htm_cell_notify_ori(cell, &pos->ori_hash);
#endif
	if (h_ori)
		htm_layer_set(ori_l, h_ori);

	/* Location (L6b) -> Feature@Location (L5b) */
//	htm_cell_notify_loc(cell, &pos->loc_hash);
	htm_layer_set(seg_l, &loc_l->hash);

	/* Inhibits/resets stimuli input. 
	htm_chord_inhibit(&prefe_l->hash);
	*/

}
#endif

int htm_cell_motor_apply(entity_t *ent, brane_t *br, cell_t *cell, chord_t *h_inset, chord_t *h_ori)
{
	layer_t *ori_l = &cell->layer[CELL_L6a]; /* Orientation (motor/axis) */
	layer_t *loc_l = &cell->layer[CELL_L6b]; /* Location (motor/grid) */
	layer_t *seg_l = &cell->layer[CELL_L5b]; /* Feature@Location (seq) */

	if (h_inset)
		htm_layer_set(loc_l, h_inset);
	if (h_ori)
		htm_layer_set(ori_l, h_ori);

	htm_layer_set(seg_l, &loc_l->hash);
}

/**
 * stimuli received directly from a brane (analogous with cortex).
 * Input: Applied to neocortex layer 2.
 * Output: Feedback is returned to brane from layer 3.
 */
int htm_cell_brane_notify(entity_t *ent, brane_t *br, cell_t *cell, chord_t *h_data)
{
	layer_t *prefe_l = &cell->layer[CELL_L2]; /* Feature (seq) */
	layer_t *fe_l = &cell->layer[CELL_L3]; /* Feature (pool) */

//	if (!htm_cell_notify_feature(cell, h_data)) return (0);
	if (!htm_layer_set(prefe_l, h_data))
		return (0);
	htm_layer_set(fe_l, &prefe_l->hash);

	if (br->def && br->def->write) {
		(*br->def->write)(ent, br, cell);
	}

	return (1);
}

chord_t *htm_cell_feature(cell_t *cell)
{
	layer_t *fe_l = &cell->layer[CELL_L3]; /* Feature (pool) */
	return (&fe_l->hash);
}

chord_t *htm_cell_object(cell_t *cell)
{
	layer_t *obj_l = &cell->layer[CELL_L5a];
	return (&obj_l->hash);
}

chord_t *htm_cell_orientation(cell_t *cell)
{
	layer_t *ori_l = &cell->layer[CELL_L6a]; /* Orientation (motor/axis) */
	return (&ori_l->hash);
}

chord_t *htm_cell_location(cell_t *cell)
{
	layer_t *ori_l = &cell->layer[CELL_L6b]; /* Location (motor/grid) */
	return (&ori_l->hash);
}

uint64_t htm_cell_csum(cell_t *cell, uint8_t *data, size_t data_len)
{
	return (crc64(cell->id, data, data_len));
}

uint64_t htm_cell_bias(cell_t *cell)
{
	return (cell->bias);
}

void htm_cell_bias_set(cell_t *cell, uint64_t bias)
{
	cell->bias = bias;
}

chord_t *htm_cell_hash(cell_t *cell)
{
	static chord_t ret_hash;

	memset(&ret_hash, 0, sizeof(ret_hash));
	htm_chord_expand(&ret_hash, cell->id);

	return ((chord_t *)&ret_hash);
}

/**
 * Generate a new cell which reads input from a parent cell and provides feedback.
 */
cell_t *htm_cell_derive(brane_t *br, cell_t *cell)
{
	cell_t *ret_cell;
	path_t *path;

	ret_cell = htm_cell_init(br);
	if (!ret_cell)
		return (NULL);

	path = htm_path_new(br, cell, ret_cell);
	if (!path)
		return (NULL);

//	path->flag |= PATH_DERIVE;

	return (ret_cell);
}

int htm_cell_wake(entity_t *ent, brane_t *br, cell_t *cell)
{
	chord_t blank_hash;

	memset(&blank_hash, 0, sizeof(blank_hash));
	return (htm_cell_notify(ent, br, cell, &blank_hash));
}

#if 0
typedef struct hier_t
{
	cell_t *cell[CHORD_SIZE];
} hier_t;

/**
 * Digest binary input into a cluster of cells.
 */
void htm_cell_notify_digest(hier_t *hier, uint8_t *data, size_t data_len)
{
	chord_t hash;
	size_t of;
	int idx;

	for (of = 0; of < data_len; of += CHORD_SIZE) {
		memset(&hash, 0, sizeof(hash));
		htm_cell_digest(&hash, (data + of), MIN(CHORD_SIZE, (data_len - of)));
		for (idx = 0; idx < CHORD_SIZE; idx++) {
			htm_cell_notify(ent, br, hier->cell[idx], &hash, h_inset, h_ori);
		}
	}

}
#endif

void htm_cell_inhibit(cell_t *cell)
{
	int i;

	for (i = 0; i < MAX_CELL_LAYER; i++) {
		htm_layer_inhibit(&cell->layer[i]);
	}

}

void htm_cell_rgba(cell_t *cell, uint8_t *rgba)
{
	chord_t *hash;

	hash = htm_cell_object(cell);
	htm_chord_rgba(hash, rgba);
}

void htm_cell_rgba_set(cell_t *cell, uint8_t *rgba)
{
	chord_t *hash;

	hash = htm_cell_object(cell);
	htm_chord_rgba_set(hash, rgba);
}

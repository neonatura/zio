
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

static brane_def_t brane_def_table[MAX_ENTITY_BRANES] = {
  { BR_CONTROL, "control", 1048576, htm_control_brane_init, htm_control_brane_run, htm_control_brane_term },
  { BR_NEO, "neo", 8388608, htm_neo_brane_init },
  { BR_SENSE, "sense", 16777216, htm_sense_brane_init },
  { BR_VISION, "vision", 59444100 },
  { BR_AUDIO, "audio", 8388608 },
  { BR_SMELL, "smell", 4194304 },
  { BR_VIRTUAL, "virtual", 8388608 },
  { BR_MOTOR, "motor", 8388608 },
  { BR_EMOTION, "emotion", 8388608, htm_emo_brane_init, htm_emo_brane_run, htm_emo_brane_term },
  { BR_EXEC, "exec", 8388608, htm_exec_brane_init }
};

int htm_entity_init(entity_t **ent_p, const char *name)
{
	entity_t *ent;
	int i;

	ent = (entity_t *)calloc(1, sizeof(entity_t));
	if (!ent)
		return (-ENOMEM);

	strncpy(ent->name, name, sizeof(ent->name)-1);
	ent->id = crc64(0, ent->name, strlen(ent->name));

	/* initialize neocortical branes */
	for (i = 0; i < MAX_ENTITY_BRANES; i++) {
		ent->brane[i] = htm_brane_init(ent->id,
				brane_def_table[i].label, brane_def_table[i].size); 
		if (ent->brane[i]) ent->brane[i]->type = i;
		if (ent->brane[i]) ent->brane[i]->def = &brane_def_table[i];
		if (ent->brane[i]) ent->brane[i]->ent = ent;
	}

	/* initialize "primary" cell for branes. */
	for (i = 0; i < MAX_ENTITY_BRANES; i++) {
		if (!ent->brane[i]) continue;
		ent->brane_cell[i] = htm_cell_init(ent->brane[i]);
	}

	/* initialize subcortical branes */
	for (i = 0; i < MAX_ENTITY_BRANES; i++) {
		if (!ent->brane[i] || !ent->brane[i]->def || !ent->brane[i]->def->init)
			continue;
		(*ent->brane[i]->def->init)(ent, ent->brane[i]);
	}

	ent->sess_stamp = ztime_now() - 15;
	ent->sess_id = ~0;
	ent->sess_rate = 0.3;

	ent->path_map = hmap_init();

	*ent_p = ent;
	return (0);
}

void htm_entity_free(entity_t **ent_p)
{
	entity_t *ent;
	int i;

	if (!ent_p)
		return;

	ent = *ent_p;
	*ent_p = NULL;
	if (!ent)
		return;

	for (i = 0; i < MAX_ENTITY_BRANES; i++) {
		htm_brane_free(&ent->brane[i]);
	}

	hmap_free(&ent->path_map);

	free(ent);
}

#if 0
path_t *htm_entity_path(entity_t *ent, cell_t *cell)
{
	path_t *p;

	p = (path_t *)hmap_get(ent->path_map, htm_cell_hash(cell));
	if (!p)
		return (NULL);

	return (p);
}

path_t *htm_entity_path_set(entity_t *ent, cell_t *cell, cell_t *out_cell)
{
	chord_t *cell_hash = htm_cell_hash(cell);
	chord_t *outcell_hash;
	path_t *n_p;
	path_t *p;

	n_p = (path_t *)hmap_get(ent->path_map, cell_hash);

	p = (path_t *)calloc(1, sizeof(path_t));
	if (!p)
		return (NULL);

	p->brane_type = out_cell->brane_type;
	outcell_hash = htm_cell_hash(out_cell);
	memcpy(&p->h_cell, &outcell_hash, sizeof(chord_t));

	p->next = n_p;
	hmap_set(ent->path_map, cell_hash, p);
	
	return (p);
}

void htm_entity_relay(entity_t *ent, cell_t *cell, path_t *path)
{
	layer_t *ori_l = &cell->layer[CELL_L6a]; /* Orientation (motor/axis) */
	layer_t *loc_l = &cell->layer[CELL_L6b]; /* Location (motor/grid) */
	layer_t *in_l = &cell->layer[CELL_L4]; /* Sensation@Orientation (Seq) */
	brane_t *br;
	cell_t *o_cell;
	path_t *p;

	/* for each path send 'Object' to output cell's "Input". */
	for (p = path; p; p = p->next) {
		br = htm_entity_brane(ent, path->brane_type);
		if (!br) continue;
		o_cell = htm_brane_map_get(br, &path->h_cell);
		if (!o_cell) continue;
		htm_cell_notify(ent, o_cell, &in_l->hash, &loc_l->hash, &ori_l->hash);
	}

}
#endif


brane_t *htm_brane_get(entity_t *ent, int br_idx)
{

	if (br_idx < 0 || br_idx >= MAX_ENTITY_BRANES)
		return (NULL);

	return (ent->brane[br_idx]);
}

int htm_entity_notify(entity_t *ent, brane_t *br, int type, int subtype, chord_t *h_data, chord_t *h_inset, chord_t *h_ori)
{
	cell_t *cell;

	cell = htm_control_cell(ent, type, subtype);
	if (!cell)
		return (0);

	htm_cell_motor_apply(ent, br, cell, h_inset, h_ori);
	return (htm_cell_notify(ent, br, cell, h_data));
}




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
 *  libhtm is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with libhtm.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "htm.h"

pos_t *htm_cell_position(cell_t *cell)
{
	static pos_t ret_pos;
	layer_t *ori_l = &cell->layer[CELL_L6a]; /* Orientation (motor/axis) */
	layer_t *loc_l = &cell->layer[CELL_L6b]; /* Location (motor/grid) */
	double loc[6];
	double ori[6];

	memset(&ret_pos, 0, sizeof(ret_pos));

	memcpy(&ret_pos.loc_hash, &loc_l->hash, sizeof(chord_t));
	memcpy(&ret_pos.ori_hash, &ori_l->hash, sizeof(chord_t));

	htm_chord_split_get(&ret_pos.loc_hash, 6, (double *)loc, MIN_CHORD_SPLIT_SIZE);
	htm_chord_split_get(&ret_pos.ori_hash, 6, (double *)ori, MIN_CHORD_SPLIT_SIZE);

	ret_pos.loc_x = loc[0];
	ret_pos.loc_y = loc[1];
	ret_pos.loc_z = loc[2];
//	ret_pos.loc_velocity = loc[3];

	ret_pos.ori_x = ori[0];
	ret_pos.ori_y = ori[1];
	ret_pos.ori_z = ori[2];
//	ret_pos.ori_force = ori[3];

	return ((pos_t *)&ret_pos);
}

void htm_position_gen(pos_t *pos)
{
	double loc[6];
	double ori[6];

	loc[0] = pos->loc_x;
	loc[1] = pos->loc_y;
	loc[2] = pos->loc_z;
	loc[3] = 0;
	loc[4] = 0;
	loc[5] = 0;

	ori[0] = pos->ori_x;
	ori[1] = pos->ori_y;
	ori[2] = pos->ori_z;
	ori[3] = 0;
	ori[4] = 0;
	ori[5] = 0;

	htm_chord_split_set(&pos->loc_hash, 6, (double *)loc, MIN_CHORD_SPLIT_SIZE);
	htm_chord_split_set(&pos->ori_hash, 6, (double *)ori, MIN_CHORD_SPLIT_SIZE);

}

pos_t *htm_cell_position_translate(cell_t *cell, chord_t *h_inset, chord_t *h_ori)
{
	static pos_t ret_pos;
	double loc[6];
	double ori[6];
	int i;

	memset(&ret_pos, 0, sizeof(ret_pos));
	
	for (i = 0; i < 6; i++) loc[i] = 0;
	if (h_inset) htm_chord_split_get(h_inset, 6, (double *)loc, MIN_CHORD_SPLIT_SIZE);
	ret_pos.loc_x = loc[0] + cell->loc_x;
	ret_pos.loc_y = loc[1] + cell->loc_y;
	ret_pos.loc_z = loc[2] + cell->loc_z;
//	ret_pos.loc_velocity = loc[3];

	for (i = 0; i < 6; i++) ori[i] = 0;
	if (h_ori) htm_chord_split_get(h_ori, 6, (double *)ori, MIN_CHORD_SPLIT_SIZE);
	ret_pos.ori_x = ori[0] + cell->ori_x;
	ret_pos.ori_y = ori[1] + cell->ori_y;
	ret_pos.ori_z = ori[2] + cell->ori_z;
//	ret_pos.ori_force = ori[4];

	htm_position_gen(&ret_pos);
	return ((pos_t *)&ret_pos);
}


/* add a feature to a grid */
int htm_grid_add(grid_t *grid, chord_t *feature)
{
}

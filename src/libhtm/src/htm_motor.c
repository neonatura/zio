

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


/*
 * @param force A value from 0 to 1.
 * @param z_axis A value from 0 to 1 indicationg left-to-right axis with front being 0.25 (90 degrees).
 */
void htm_motor_code(chord_t *ret_hash, double force, double z_axis)
{
	chord_t hash;
	uint64_t f_sum;

	memset(&hash, 0, sizeof(hash));

	f_sum = (force * (double)UINT32_MAX);
	f_sum *= (z_axis * (double)UINT32_MAX);
	htm_chord_expand(&hash, f_sum);

	htm_chord_set(ret_hash, &hash);

}

double htm_motor_code_force(chord_t *hash)
{
	uint64_t f_sum;

	f_sum = htm_chord_compact(hash);
	f_sum = (f_sum % UINT32_MAX);

	return ((double)f_sum / (double)UINT32_MAX);
}

double htm_motor_code_axis(chord_t *hash)
{
	uint64_t f_sum;

	f_sum = htm_chord_compact(hash);
	f_sum /= UINT32_MAX;  

	return ((double)f_sum / (double)UINT32_MAX);
}

/**
 * Find average z-axis direction for cluster of cells.
 * @returns A value from 0 to 1 indicatio degree of direction.
 */
double htm_motor_cluster_axis(tree_t *tree)
{
	node_t *node;
	double axis;
	int total;
	int idx;

	axis = 0;
	total = 0;
	for (idx = 0; idx < MAX_CACHE_BLOCK_SIZE; idx++) {
		for (node = tree->node_list[idx]; node; node = node->next) {
			chord_t *hash = htm_cell_orientation(node->cell);
			axis += htm_motor_code_axis(hash);
			total++;
		}
	}
	axis /= (double)total;

	return (axis);
}

void htm_motor_plan_queue(entity_t *ent, cell_t *cell)
{
}

/**
 * Introduce a sequence of motor codes that are planned to be performed.
 */
void htm_motor_plan(entity_t *ent, tree_t *tree)
{
	brane_t *br;
	node_t *node;
	int idx;

	br = htm_brane_get(ent, BR_MOTOR);
	if (!br)
		return;

	for (idx = 0; idx < MAX_CACHE_BLOCK_SIZE; idx++) {
		for (node = tree->node_list[idx]; node; node = node->next) {
			/* buffer sequences into preplan queue/blog. */
			htm_motor_plan_queue(ent, node->cell);
		}
	}

}

/* The boundary extents relative to the center of mass. */
void htm_motor_extent(entity_t *ent, double *x_p, double *y_p, double *z_p)
{
	cell_t *pr_cell = ent->brane_cell[BR_MOTOR];
	pos_t *pos = htm_cell_position(pr_cell);

	*x_p = pos->loc_x;
	*y_p = pos->loc_y;
	*z_p = pos->loc_z;
}

void htm_motor_extent_set(entity_t *ent, double x, double y, double z)
{
	cell_t *pr_cell = ent->brane_cell[BR_MOTOR];
	layer_t *loc_l = &pr_cell->layer[CELL_L6b]; /* Location (motor/grid) */
	pos_t *pos = htm_cell_position(pr_cell);

	if (x < pos->loc_x &&
			y < pos->loc_y &&
			z < pos->loc_z)
		return;

	pos->loc_x = MAX(x, pos->loc_x);
	pos->loc_y = MAX(y, pos->loc_y);
	pos->loc_z = MAX(z, pos->loc_z);
	htm_position_gen(pos);

	htm_chord_set(&loc_l->hash, &pos->loc_hash);
}

/* Center X/Y/Z Axis to entity's control position. */
void htm_motor_center(entity_t *ent)
{
	cell_t *pr_cell = ent->brane_cell[BR_MOTOR];
	pos_t *pos = htm_cell_position(pr_cell);
	double x_axis = (pos->ori_x * 360);
	double y_axis = (pos->ori_y * 360);
	double z_axis = (pos->ori_z * 360);
	double diff;
	double of;

	/* center x-axis, sway side-to-side */
	diff = (ent->control.x_axis - x_axis);
	if (diff < 0) diff = (360 - diff);
	if (x_axis >= diff) {
		htm_motor_sway(ent, X_AXIS, (diff - 90) / 90);
	} else {
		htm_motor_sway(ent, X_AXIS, (90 - diff) / 90);
	}

	/* center y-axis, sway back and forth */
	diff = (ent->control.y_axis - y_axis);
	if (diff < 0) diff = (360 - diff);
	if (y_axis >= diff) {
		htm_motor_sway(ent, Y_AXIS, (diff - 90) / 90);
	} else {
		htm_motor_sway(ent, Y_AXIS, (90 - diff) / 90);
	}

	/* center z-axis, pan left-right */
	diff = (ent->control.z_axis - z_axis);
	if (diff < 0) diff = (360 - diff);
	if (z_axis >= diff) {
		htm_motor_sway(ent, Z_AXIS, (diff - 90) / 90);
	} else {
		htm_motor_sway(ent, Z_AXIS, (90 - diff) / 90);
	}

}

/* sway: move or cause to move slowly or rhythmically backward and forward or from side to side. */
void htm_motor_sway(entity_t *ent, int axis, double speed)
{

}


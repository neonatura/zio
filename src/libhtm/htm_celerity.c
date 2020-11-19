

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

/* The celerity brane is based on the cerebellum.
 *
 * When a motor action is intended to be performed, that action is relayed as a 'celerity plan' event, and when the motor action has been performed, the actual end-result position of the motor command is recorded.
 *
 * When the resulting position and the intended position differ, an error is assumed to have been made.
 *
 * When a similar movement is being planned that contains an error, the specific error bits will be removed based on the difference between the origin motor command the resulting one.
 *
 * ? Multiple motor codes may be combined in bursts in order to cordinate movements between different joints. Error states should interupt the current timed sequence, in this case.  
 */

cell_t *htm_celerity_cell(brane_t *br, chord_t *h_data)
{
	tree_t *tree;
	cell_t *cell;
	uint64_t key;

#if 0
	chord_t hash;
	memcpy(&hash, h_data, sizeof(chord_t));
	htm_chord_xor(&hash, h_loc);
#endif
	key = htm_chord_compact(h_data);

	tree = htm_brane_cache(br);
	cell = htm_tree_cache_get(tree, key);
	if (!cell) {
		cell = htm_cell_init(br);
		if (!cell) 
			return (NULL);

		htm_tree_cache_set(tree, key, cell);
	}

	return (cell);
}

/**
 * @params h_data
 *   destination location or obj
 *   or
 *   destination obj@loc, 
 *
 * @params h_loc current location
 */
int htm_celerity_motion_plan(entity_t *ent, chord_t *cur_loc, chord_t *next_loc)
{
	static chord_t blank_hash;
	brane_t *br_motor = htm_brane_get(ent, BR_MOTOR);
	cell_t *cell = htm_celerity_cell(br_motor, cur_loc);
	chord_t *inv_loc;
	int valid;

	inv_loc = htm_cell_location(cell);

	/* determine if error state already exists for motor command. */
	if (htm_chord_match(next_loc, inv_loc)) {
		return (FALSE);
	} 

//	htm_cell_motor_apply(ent, br_motor, cell, next_loc, NULL);
	htm_cell_notify(ent, br_motor, cell, next_loc);

	return (TRUE);
}

void htm_celerity_motion_commit(entity_t *ent, chord_t *last_loc, chord_t *cur_loc)
{
	brane_t *br_motor = htm_brane_get(ent, BR_MOTOR);
	cell_t *cell = htm_celerity_cell(br_motor, last_loc);
	chord_t n_loc;

	//memcpy(&n_loc, htm_cell_object(cell), sizeof(chord_t));
	memcpy(&n_loc, htm_cell_feature(cell), sizeof(chord_t));

	/* compare intended destination location to actual location established. */
	if (htm_chord_match(cur_loc, &n_loc)) {
		/* inhibit due to no error state */
		htm_cell_inhibit(cell);
		return;
	}

	/* retain mask to remove error state. */
	chord_t hash;
	memcpy(&hash, &n_loc, sizeof(chord_t));
//	htm_chord_xor(&hash, cur_loc);
	htm_chord_unset(&hash, cur_loc); /* clear bits set in cur_loc */


	//htm_cell_notify(ent, br_motor, cell, &hash);
	htm_cell_motor_apply(ent, br_motor, cell, &hash, NULL);

}

void htm_celerity_motion_apply(entity_t *ent, chord_t *h_data, chord_t *h_loc)
{

	/*
	htm_motor_apply(ent, h_data, h_loc);
	*/
}



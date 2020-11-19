
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
 * The neo brane emulates the function of the thalamus, and provides a "gate keeper" for all auditory, visual, emotional, and control information to the remaining branes.
 * The control brane can modulate the degree at which activity occurs in the neo brane in order to amplify feed-foward action in response to stimuli or depress incoming stimuli for when the entity is in a idle or sleeping state.
 */ 

static hmap_t *_sequence_table;

void htm_neo_brane_init(entity_t *ent, brane_t *br)
{
	unsigned char buf[8];
	unsigned char ch;

	_sequence_table = hmap_init();

	/* generate a schema of all 7-bit ascii characters. */
	for (ch = 32; ch < 128; ch++) {
		memset(buf, 0, sizeof(buf));
		buf[0] = ch;
		htm_neo_seq_set(br, buf, 1);
	}

}

void htm_neo_brane_term(brane_t *br, cell_t *pr_cell)
{
	hmap_free(&_sequence_table);
}

void htm_neo_seq_set(brane_t *br, uint8_t *data, size_t data_len)
{
	cell_t *cell;
	chord_t hash;
	uint32_t seed = 0;

	cell = htm_cell_init(br);
	if (!cell)
		return;
	
	memset(&hash, 0, sizeof(hash));
	htm_chord_digest(&hash, &seed, data, data_len);
	hmap_set_chord(_sequence_table, &hash, cell); 
}

cell_t *htm_neo_seq(brane_t *br, uint8_t *data, size_t data_len)
{
	cell_t *cell;
	chord_t hash;
	uint32_t seed = 0;

	memset(&hash, 0, sizeof(hash));
	htm_chord_digest(&hash, &seed, data, data_len);
	cell = (cell_t *)hmap_get_chord(_sequence_table, &hash);
	if (!cell)
		return (NULL);

	return (cell);
}

#if 0
/* neo brane channels provides a method of communication between branes */
cell_t *htm_neo_chan_out(entity_t *ent, uint64_t chan_id, cell_t *cell)
{
	brane_t *br = htm_brane_get(ent, BR_NEO);

	cell = (cell_t *)hmap_get(_channel_table, chan_id);
	if (!cell) {
		cell = htm_cell_init(br);
		if (!cell)
			return (NULL);
		hmap_get(_channel_table, chan_id, cell);
	}

	return (NULL);
}

cell_t *htm_neo_chan_in(entity_t *ent, uint64_t id)
{
	brane_t *br = htm_brane_get(ent, BR_NEO);
	return (NULL);
}

#endif


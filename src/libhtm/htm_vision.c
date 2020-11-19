
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

// note: if 'all' pixels move then body is moving (can be used to filter out self-movement by negating 'large scale movemnts' from detailed ones

void htm_vision_cell_pos_init(entity_t *ent, brane_t *br)
{
	cell_t *pr_cell = ent->brane_cell[BR_VISION];
	cell_t *cells[64][64];
	cell_t *line_cells[8192];
	int y, x;
	int idx;

	/* dot */
	for (y = 0; y < 64; y++) {
		for (x = 0; x < 64; x++) {
			cells[y][x] = htm_cell_init(br);  
		}
	}

	/* line */
	idx = 0;
	for (y = 0; y < 64; y++) {
		for (x = 0; x < 64; x++) {
			int left_x = (x >= 2) ? (d_x - 1) : 63;
			int up_y = (y >= 2) ? (d_y - 1) : 63;

			cell = htm_cell_init(br);
			htm_path_new(br, cells[d_y][left_x], cell);
			htm_path_new(br, cells[d_y][d_x], cell);
			line_cells[idx++] = cell;

			cell = htm_cell_init(br);
			htm_path_new(br, cells[up_y][d_x], cell);
			htm_path_new(br, cells[d_y][d_x], cell);
			line_cells[idx++] = cell;
		}
	}

	/* curve/angle */
	idx = 0;
	for (y = 0; y < 128; y++) {
		for (x = 0; x < 128; x++) {
			int left_x = (x >= 2) ? (x/2 - 1) : 63;
			int up_y = (y >= 2) ? (y/2 - 1) : 63;

			cell = htm_cell_init(br);
			htm_path_new(br, line_cells[y][left_x], cell);
			htm_path_new(br, cells[up_y][x], cell);
			htm_path_new(br, cell, pr_cell);

			cell = htm_cell_init(br);
			htm_path_new(br, line_cells[up_y][x], cell);
			htm_path_new(br, cells[y][left_x], cell);
			htm_path_new(br, cell, pr_cell);
		}
	}

}

cell_t *htm_vision_cell_pos(entity_t *ent, int x, int y)
{
	brane_t *br;
	tree_t *cache;
	cell_t *cell;
	pos_t tpos;
	uint64_t key;

	br = htm_brane_get(ent, BR_VISION);
	if (!br)
		return;

	memset(&tpos, 0, sizeof(tpos));
	tpos.loc_x = x;
	tpos.loc_y = y;
	htm_cell_position_gen(&tpos);

	cache = htm_brane_cache(br);
	key = htm_chord_compact(&pos->loc_hash);
	cell = htm_tree_cache_get(cache, key);
	if (!cell) {
		cell = htm_cell_init(br);
		htm_tree_cache_set(cache, key, cell);
	}

	return (cell);
}

void htm_vision_brane_init(entity_t *ent, brane_t *br)
{

	htm_vision_cell_pos_init(ent, br);

}

void *htm_vision_brane_run(entity_t *ent)
{

}

void htm_vision_brane_term(entity_t *ent, brane_t *br)
{
}

void htm_vision_brane_wake(entity_t *ent, brane_t *br)
{

}

/* break down matrix into dots. */
void htm_vision_reduce_dots(entity_t *ent, image_t *image)
{
	const double contrast = htm_control_contrast(ent);
	brane_t *br;
	cell_t *cells[4096];
	chord_t h_data; 
	int x, y;
	int idx;

	br = htm_brane_get(ent, BR_VISION);
	if (!br)
		return;


	idx = 0;
	for (y = 0; y < image->width; y++) {
		for (x = 0; x < image->height; x++) {
			cells[idx] = htm_vision_cell_pos(ent, x, y);

			memset(&pos, 0, sizeof(pos));
			pos.loc_x = x;
			pos.loc_y = y;
			htm_cell_position_gen(cells[idx], &pos);
			htm_cell_motor_apply(ent, br, cells[idx], &pos.loc_hash, &pos.ori_hash);

			idx++;
		}
	}

	for (y = 0; y < image->width; y++) {
		for (x = 0; x < image->height; x++) {
			const uint8_t *ival = (image->pixel_data[(y*image->width+x)*image->bytes_per_pixel]);
			uint32_t val = (ival[0] + ival[1] + ival[2]) * ival[3];
			if (val > ((double)UINT32_MAX * contrast)) {
				memset(&h_data, 0, sizeof(h_data));
				htm_chord_rgba_set(&h_data, ival);
				htm_cell_notify(ent, br, cells[idx], &h_data);
			}
			idx++;
		}
	}

}

void htm_vision_primary_color(entity_t *ent, uint8_t *rgba)
{
	cell_t *pr_cell;
	
	pr_cell = ent->brane_cell[BR_VISION];
	if (!pr_cell)
		return (NULL);

	htm_cell_rgba(pr_cell, rgba);
}


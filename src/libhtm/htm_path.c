
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


chord_t *htm_path_bias_csum(path_t *path)
{
	static chord_t ret_hash;
	memset(&ret_hash, 0, sizeof(ret_hash));
	htm_chord_expand(&ret_hash, path->bias_csum);
	return ((chord_t *)&ret_hash);
}

double htm_path_bias_weight(path_t *path)
{
	return (path->bias_weight);
}

//void htm_path_weight_set(path_t *path, double weight)
void htm_path_bias_set(path_t *path, chord_t *hash, double weight)
{
	uint64_t csum;

	if (hash)
		path->bias_csum = htm_chord_compact(hash);
	else
		path->bias_csum = 0;

	path->bias_weight = weight;
}

cell_t *htm_path_cell(brane_t *br, path_t *path)
{
	return ((cell_t *)&br->cell[path->cell_idx]);
}

path_t *htm_path_first(brane_t *br, cell_t *cell)
{

	if (cell->path_idx == 0)
		return (NULL);
//fprintf(stderr, "DEBUG: htm_path_first: path_idx %d\n", cell->path_idx);
	if (cell->path_idx == cell->cell_idx)
		return (NULL);
	return (&br->path[cell->path_idx]);
}

path_t *htm_path_next(brane_t *br, cell_t *cell, path_t *path)
{

	if (path->next_idx == 0)
		return (NULL);


	path_t *p = (path_t *)&br->path[path->next_idx];

	if (p->cell_idx == cell->cell_idx)
		return (NULL);

//fprintf(stderr, "DEBUG: htm_path_next: path %d -> %d\n", path->path_idx, p->path_idx);
	return (p);
//	(path_t *)&br->path[path->next_idx]);
}

path_t *htm_path_init(brane_t *br)
{
	path_t *path;
	uint64_t br_id;
	int idx;

	if (br->path_len + 1 >= br->path_max)
		return (NULL);

	path = &br->path[br->path_len];
	memset(path, 0, sizeof(path_t));
	path->path_idx = br->path_len;
	br->path_len++;

	return (path);
}

path_t *htm_path_new(brane_t *br, cell_t *in, cell_t *out)
{
	path_t *p;

	p = htm_path_init(br);
	if (!p)
		return (NULL);

	/* output cell */
	p->cell_idx = out->cell_idx;

	/* link list */
	p->next_idx = in->path_idx;
	in->path_idx = p->path_idx;

	return (p);
}


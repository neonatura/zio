
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

#ifndef __HTM_PATH_H__
#define __HTM_PATH_H__

chord_t *htm_path_bias_csum(path_t *path);

double htm_path_bias_weight(path_t *path);

void htm_path_bias_set(path_t *path, chord_t *hash, double weight);

cell_t *htm_path_cell(brane_t *br, path_t *path);

path_t *htm_path_init(brane_t *br);

path_t *htm_path_new(brane_t *br, cell_t *in, cell_t *out);

path_t *htm_path_init(brane_t *br);

void htm_path_weight_set(path_t *path, double weight);

path_t *htm_path_first(brane_t *br, cell_t *cell);

path_t *htm_path_next(brane_t *br, cell_t *cell, path_t *path);

#endif /* ndef __HTM_PATH_H__ */


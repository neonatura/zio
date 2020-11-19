
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

#ifndef __HTM_CELL_H__
#define __HTM_CELL_H__

cell_t *htm_cell_init(brane_t *br);

/**
 * @param inset The location of input-sensor relative to the encapsulating object.
 * @param ori A checksum representing the input-sensor orientation.
 * @param data The contextual input data of the sensation.
 */
int htm_cell_notify(entity_t *ent, brane_t *br, cell_t *cell, chord_t *h_data);

/* update cell with entity location and orientation. */
int htm_cell_motor_apply(entity_t *ent, brane_t *br, cell_t *cell, chord_t *h_inset, chord_t *h_ori);

uint64_t htm_cell_csum(cell_t *cell, uint8_t *data, size_t data_len);

chord_t *htm_cell_object(cell_t *cell);

uint64_t htm_cell_bias(cell_t *cell);

void htm_cell_bias_set(cell_t *cell, uint64_t bias);

chord_t *htm_cell_hash(cell_t *cell);

cell_t *htm_cell_derive(brane_t *br, cell_t *cell);

int htm_cell_wake(entity_t *ent, brane_t *br, cell_t *cell);

void htm_cell_inhibit(cell_t *cell);

chord_t *htm_cell_orientation(cell_t *cell);

void htm_cell_rgba(cell_t *cell, uint8_t *rgba);

void htm_cell_rgba_set(cell_t *cell, uint8_t *rgba);

chord_t *htm_cell_location(cell_t *cell);

chord_t *htm_cell_feature(cell_t *cell);

#endif /* ndef __HTM_CELL_H__ */



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

#ifndef __HTM_LAYER_H__
#define __HTM_LAYER_H__

/**
 * @returns TRUE on sub-match of hash and FALSE otherwise.
 */
int htm_layer_verify(layer_t *l, chord_t *hash);

void htm_layer_inhibit(layer_t *l);

/* apply a new hash to layer. */
int htm_layer_set(layer_t *l, chord_t *hash);

int htm_layer_spike(layer_t *l);

void htm_layer_inhibit(layer_t *l);

void htm_layer_excite(layer_t *l);

void htm_layer_level_incr(layer_t *hash, int idx);

void htm_layer_level_decr(layer_t *l, int idx);

#endif /* ndef __HTM_LAYER_H__ */



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

void htm_op_feedback(layer_t *l, chord_t *in_hash, int flag)
{
	int i;

  for (i = 0; i < CHORD_SIZE; i++) {
		if (in_hash->vec[i] == 0) {
			htm_layer_level_decr(l, i);
		} else {
			htm_layer_level_incr(l, i);
		}
  }

}


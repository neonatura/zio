/*
 * @copyright
 *
 *  Copyright 2020 Neo Natura
 *
 *  This file is part of the zio project.
 *  (https://github.com/neonatura/zio)
 *
 *  The zio project is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The zio project is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with zio project.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  @endcopyright
 */

#include "ziod.h"

extern entity_t *enttity;

int ziod_therm_notify(zdev_t *dev)
{
	brane_t *br = htm_brane_get(entity, BR_SENSE);
	double avg_val = zio_dvalue_avg(dev, MAX_VALUE_DOUBLE_SIZE);
	double val = zio_dvalue_get(dev);
	double diff_val = abs(avg_val - val);
	chord_t hash;
	chord_t pos;

fprintf(stderr, "DEBUG: ziod_therm_notify: '%s'\n", dev->label); 

	if (val != 0.00) {
		chord_t *hash = htm_sense_hash(val, 0, 0);
fprintf(stderr, "DEBUG: ziod_therm_cycle: hash \"%s\" (val:  %f)\n", htm_chord_hex(hash), val); 
		htm_sense_notify_internal(entity, hash);
	}

}

/*
 *  Copyright 2020 Brian Burrell
 *
 *  This file is part of zio project.
 *  (https://github.com/neonatura/zio)
 *
 *  zio project is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  zio project is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with zio project.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __HTM_SENSE_H__
#define __HTM_SENSE_H__

#define MIN_SENSE_TEMP -35
#define MAX_SENSE_TEMP 50
#define MIN_SENSE_PRESSURE 100
#define MAX_SENSE_PRESSURE 10000

/** Initialize an entity's sensory brane. */
void htm_sense_brane_init(entity_t *ent, brane_t *br);

/**
 * Generate a chord hash representing sensory information.
 *
 * @param temp Thermal temperature (celcius).
 * @param pressure External pressure applied (pascals).
 * @param prop Stretchability (percentage).
 * @note A value of zero does not contribute to the hash returned.
 */
chord_t *htm_sense_hash(double temp, double pressure, double prop);

/** Process incoming internal stimuli. */
void htm_sense_notify_internal(entity_t *ent, chord_t *hash);

#endif /* ndef __HTM_SENSE_H__ */

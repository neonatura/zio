

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

#ifndef __HTM_VISION_H__
#define __HTM_VISION_H__

/* vision: scans area for features. either matches simple features to objects or sums features to generate object. */

void htm_vision_brane_init(entity_t *ent, brane_t *br);

void *htm_vision_brane_run(entity_t *ent);

void htm_vision_brane_term(entity_t *ent, brane_t *br);

void htm_vision_brane_wake(entity_t *ent, brane_t *br);

void htm_vision_primary_color(entity_t *ent, uint8_t *rgba);

#endif /* ndef __HTM_VISION_H__ */

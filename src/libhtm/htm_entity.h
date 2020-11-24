
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

#ifndef __HTM_ENTITY_H__
#define __HTM_ENTITY_H__

#define ENTITY_ZPU(_ent) \
	(&(_ent)->zpu)

int htm_entity_init(entity_t **ent_p, const char *name, zprocessor_t *zproc);

void htm_entity_free(entity_t **ent_p);

brane_t *htm_brane_get(entity_t *ent, int br_idx);

int htm_entity_notify(entity_t *ent, brane_t *br, int type, int subtype, chord_t *h_data, chord_t *h_inset, chord_t *h_ori);

#endif /* ndef __HTM_ENTITY_H__ */


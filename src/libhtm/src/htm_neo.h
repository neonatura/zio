
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

#ifndef __HTM_NEO_H__
#define __HTM_NEO_H__

void htm_neo_brane_init(entity_t *ent, brane_t *br);

void htm_neo_brane_term(brane_t *br, cell_t *pr_cell);

void htm_neo_seq_set(brane_t *br, uint8_t *data, size_t data_len);

cell_t *htm_neo_seq(brane_t *br, uint8_t *data, size_t data_len);


#endif /* ndef __HTM_NEO_H__ */


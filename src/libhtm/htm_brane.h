
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

#ifndef __HTM_BRANE_H__
#define __HTM_BRANE_H__

brane_t *htm_brane_init(uint64_t ent_id, const char *tag, int size, zpu_t *zpu);

void htm_brane_free(brane_t **br_p);

const char *htm_brane_dir(void);

const char *htm_brane_fname(const char *name);

chord_t *htm_brane_map_set(brane_t *br, cell_t *cell);

cell_t *htm_brane_map_get(brane_t *br, chord_t *hash);

tree_t *htm_brane_cache(brane_t *br);

cell_t *htm_brane_cell(entity_t *ent, int br);

#endif /* ndef __HTM_BRANE_H__ */


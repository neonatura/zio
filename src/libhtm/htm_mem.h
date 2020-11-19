
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

#ifndef __HTM_MEM_H__
#define __HTM_MEM_H__

#define MAX_HTM_MEM_CACHE 256
#define MAX_HTM_MEM_MATCH 32

event_t *htm_mem_set(entity_t *ent, cell_t *cell);

void htm_mem_notify(entity_t *ent, cell_t *cell);

chord_t *htm_mem_restore(zpu_t *z, uint64_t key);

void htm_mem_relay(zpu_t *z, chord_t *hash);

chord_t *htm_mem_remember(zpu_t *z, uint64_t data);

void htm_mem_store(zpu_t *z, uint64_t key, chord_t *hash);

chord_t *htm_mem_recall(zpu_t *z, uint64_t key);

#endif /* ndef __HTM_MEM_H__ */


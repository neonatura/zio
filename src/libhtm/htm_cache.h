
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

#ifndef __HTM_CACHE_H__
#define __HTM_CACHE_H__

void htm_cache_set(cache_t *cache, uint64_t csum, chord_t *hash);

void htm_cache_sethash(cache_t *cache, chord_t *key, chord_t *hash);

chord_t *htm_cache_get(cache_t *cache, chord_t *in_hash);

#endif /* ndef __HTM_CACHE_H__ */


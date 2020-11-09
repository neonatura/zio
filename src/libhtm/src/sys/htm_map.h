
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

#ifndef __HTM_MAP_H__
#define __HTM_MAP_H__

#define INITIAL_MAX 15 /* tunable == 2^n - 1 */

hmap_t *hmap_init(void);

void hmap_free(hmap_t **meta_p);

int hmap_set(hmap_t *ht, hkey_t key, void *val);

int hmap_set_chord(hmap_t *ht, chord_t *key, void *val);

void *hmap_get(hmap_t *ht, hkey_t key);

void *hmap_get_chord(hmap_t *ht, chord_t *key);


#endif /* ndef __HTM_MAP_H__ */



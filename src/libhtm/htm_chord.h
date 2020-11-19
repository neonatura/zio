
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

#ifndef __HTM_CHORD_H__
#define __HTM_CHORD_H__

const char *htm_chord_hex(chord_t *hash);

/**
 * The orientation of the feature to the "seed" object.
 * @note The inset is relative to the seed. 
 */
uint64_t htm_chord_inset(uint64_t seed, double x, double y, double z);

uint64_t htm_chord_csum(uint8_t *data, size_t data_len);

void htm_chord_expand(chord_t *hash, uint64_t csum);

int htm_chord_match(chord_t *ch, chord_t *hash);

/**
 * Creates a 8-byte low-resolution encoding of a chord hash.
 */
uint64_t htm_chord_compact(chord_t *hash);

void htm_chord_inhibit(chord_t *hash);

double htm_chord_value(chord_t *hash);

void htm_chord_set(chord_t *hash, chord_t *in_hash);

void htm_chord_pool(chord_t *hash, chord_t *in_hash);

int htm_chord_digest(chord_t *hash, uint32_t *seed_p, uint8_t *data, size_t data_len);

void htm_chord_reset(chord_t *hash, chord_t *in_hash);

void htm_chord_reduce(chord_t *hash, double deg);

void htm_chord_split_get(chord_t *hash, int nr, double *val_p, double max_val);

void htm_chord_split_set(chord_t *hash, int nr, double *val_p, double max_val);

void htm_chord_feedback(chord_t *hash, chord_t *in_hash);

void htm_chord_rgba(chord_t *hash, uint8_t *rgba);

void htm_chord_rgba_set(chord_t *hash, uint8_t *rgba);

uint8_t htm_chord_select(uint8_t val);

double htm_chord_compare(chord_t *ch, chord_t *hash);

void htm_chord_xor(chord_t *hash, chord_t *in_hash);

void htm_chord_unset(chord_t *hash, chord_t *in_hash);

void htm_chord_merge(chord_t *hash, chord_t *in_hash);

int hmap_set_chord(hmap_t *ht, chord_t *hash, void *val);

void *hmap_get_chord(hmap_t *ht, chord_t *hash);

#endif /* ndef __HTM_CHORD_H__ */


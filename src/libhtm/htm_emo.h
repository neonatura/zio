
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

#ifndef __HTM_EMO_H__
#define __HTM_EMO_H__

/**
 * The emotional positive or negative weight associated with a sparse sequence.
 * @returns -1 (negative) to 1 (positive)
 */
double htm_emo_weight(entity_t *ent, chord_t *hash);

/**
 * Increases 'emotional weight' for a sparse sequence by one hundreth.
 */
void htm_emo_weight_incr(entity_t *ent, chord_t *hash);

/**
 * Decreases 'emotional weight' for a sparse sequence by one hundreth.
 */
void htm_emo_weight_decr(entity_t *ent, chord_t *hash);

void htm_emo_brane_init(entity_t *ent, brane_t *br);

void *htm_emo_brane_run(entity_t *ent);

void htm_emo_brane_term(entity_t *ent, brane_t *br);

double htm_emo_resonance(entity_t *ent, chord_t *hash);

double htm_emo_mood(entity_t *ent);

void htm_emo_mood_incr(entity_t *ent);

void htm_emo_mood_decr(entity_t *ent);

void htm_emo_mood_dampen(entity_t *ent);


#endif /* ndef __HTM_EMO_H__ */


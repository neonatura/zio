
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

#ifndef __HTM_CONTROL_H__
#define __HTM_CONTROL_H__

/* all input and output systems are paralyzed. entity is unconcious. */
#define STATE_DEAD 0
/* motor and somatic functions are paralyzed. entity may still be concious. */
#define STATE_DREAM 1
/* motor and somatic is dampened. entity is unconcious. */
#define STATE_SLEEP 1
/* motor and somatic is dampened. entity is 'drifting away from conciousness'. */
#define STATE_REST 2
/* motor and somatic is dampened. entity is emulating a previous event. */
#define STATE_RECALL 3 
/* motor and somatic is dampened. entity is awaiting input. */
#define STATE_IDLE 4 
/* motor and somatic is excited. entity has aroused attention. */
#define STATE_ATTENTION 5
/* entity is focusing on a particular spatial location. */
#define STATE_FOCUS_POSITION 6
/* entity is focusing on a general feature. */
#define STATE_FOCUS_FEATURE 7
/* entity is focusing on a specific object. */
#define STATE_FOCUS_OBJECT 8
/* entity is concious and without saliency. */
#define STATE_FOCUS_MEDITATE 9
#define MAX_STATE_FOCUS 10

#define BR_CONTROL_NONE 0
#define BR_CONTROL_REGISTER 1

#define BR_REG_STATE 0 /* conscious state */
#define BR_REG_STANCE 1 /* posture state */
#define BR_REG_TEMP 2 /* core temp */
#define BR_REG_EMOTE 3 /* pos/neg mood */
#define BR_REG_AIR 4 /* air quality */
#define BR_REG_POWER 5 /* energy level */

int htm_control_state(entity_t *ent);

chord_t *htm_state_hash(entity_t *ent);

/**
 * Notify primary cell of high-priority object hash.
 */
void htm_control_focus_set(entity_t *ent, chord_t *hash);

/* increase awareness of surrounding environment. */
void htm_control_unfocus(entity_t *ent);

void htm_control_brane_init(entity_t *ent, brane_t *br);

void *htm_control_brane_run(entity_t *ent);

void htm_control_brane_term(entity_t *ent, brane_t *br);

chord_t *htm_focused_location(entity_t *ent);

chord_t *htm_focused_object(entity_t *ent);

chord_t *htm_focused_feature(entity_t *ent);

/* spatial-based focused attention. "looking for something on the ground". */
void htm_control_focus_position_set(entity_t *ent, chord_t *hash);

/* general attribute based focused attention. "someone dressed in blue" */
void htm_control_focus_feature_set(entity_t *ent, chord_t *hash);

/* object based focused attention. "waiting for a grasshopper to hop" */
void htm_control_focus_object_set(entity_t *ent, chord_t *hash);

void htm_control_state_set(entity_t *ent, int state);

uint8_t htm_control_reg_get(entity_t *ent, int subtype);

cell_t *htm_control_cell(entity_t *ent, int br_type, int subtype);

#endif /* ndef __HTM_CONTROL_H__ */


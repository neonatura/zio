
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

#include "htm.h"


#define PER_LIGHT
#define PER_SOUND
#define PER_TEMP
#define PER_TASTE
#define PER_PRESSURE
#define PER_SMELL


#define MONITOR
#define ACTION
#define CONCEPT

/*
 * Flexible mapping of sensory stimuli onto action(s).
 * Short term mapping retention.
 */
#define GOAL

#define PLAN

/* Focus on an idea in working memory. */
void htm_work_add(entity_t *ent, chord_t *hash)
{

}

/* Execute an idea in the future. */
void htm_work_plan(entity_t *ent, chord_t *hash, chord_t *span)
{

}

/* Reduce an idea into seperate ideas.  */
void htm_work_reduce(entity_t *ent, chord_t *hash)
{

}

/* The importance (0 - 1) of an idea. */ 
double htm_work_gravity(entity_t *ent, chord_t *hash)
{
	return (0);
}


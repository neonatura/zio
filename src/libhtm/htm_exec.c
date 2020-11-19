
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

#if 0 
static void *_memxor(void *dest, const void *src, size_t n)
{
  char const *s = (const char *)src;
  char *d = (char *)dest;

  for (; n > 0; n--)
    *d++ ^= *s++;

  return dest;
}
#endif


/**
 * An event may trigger a 'plan' to perform a sequence of events.
 */
int htm_exec_trigger(event_t *e)
{
return (-1);
}

/* Obtain the literal text associated with a hash reference to a word. */
char *htm_exec_word_get(chord_t *chord)
{
return (NULL);
}

/* Associate a text sequence with a reference hash. */
void htm_exec_word_set(chord_t *chord, char *word)
{
}

/* Add a sequence of events to be buffered. */
void htm_exec_plan_add(ztime_t when, event_t **e)
{

}


/* facts, ideas, meanings, concepts, and places */

/* controls the various motor movements of the body */
void htm_exec_motor(entity_t *ent)
{
}


/* The limbic system is a complex system of nerves and networks in the brain, involving several areas near the edge of the cortex concerned with instinct and mood. It controls the basic emotions (fear, pleasure, anger) and drives (hunger, sex, dominance, care of offspring).  */

/* set the behavioral weight associated with a particular action. */
void htm_exec_behavior_set(chord_t *hash, double weight)
{
}

/* get the behavioral weight associated with a particular action. */
double htm_exec_behavior_get(chord_t *hash)
{
	return (0.0);
}

/* monitor's limbic system */
void htm_exec_behavior_monitor(entity_t *ent)
{
}

/* Oritofrontal Prefrontal Cortex
 * Function:
 * - Decision making
 * - Regulation of reward oriented behaviors
 * - Inhibiting inappropriate behaviors
 */

/* break down object into it's features or figure out how to execute something. */
void htm_exec_deduce(entity_t *ent)
{
}


/* dropamine (reward) functions:
 *
 * if this good for me or do i be afraid of it?
 * how good was this and do i want more,
 * but should i be doing this again ("afraid" assocation)?
 * if i should do it again, plan to perform motor task to initiate it again.
 *
 * Situation #1: Novelty response (unexpected), no prediction, reward occurs.
 * Situation #2: After learning predicted reward occurs, dopamine is released.
 * Situation #3: After learning predicted reward does not occur, cell is depressed. causes dissapointment.
 */




void htm_exec_stamp_hash(entity_t *ent, chord_t *ret_hash)
{
	chord_t hash;
	uint64_t csum;
	double freq;
	double span;
	double seed;
	double rate;
	uint32_t cseed;
	ztime_t now;
	int bit;
	int i;

	now = ztime_now();

	span = (double)(ztime_now() - ent->sess_stamp) / 100;
	seed = (double)ent->sess_id / (double)UINT_MAX;// / (double)UINT_MAX;
	rate = (double)ent->sess_rate / (double)100;

	freq = zlaplace(span, seed, rate);

	if (freq != ent->sess_freq) {
		/* remove previous timestamp frequency */
		cseed = 0;
		csum = (uint64_t)((1 + ent->sess_freq) * (double)(UINT_MAX/2));
		memset(&hash, 0, sizeof(hash));
		htm_chord_expand(&hash, csum);
		htm_chord_xor(ret_hash, &hash);
	}


	cseed = 0;
	ent->sess_freq = freq;
	csum = (uint64_t)((1 + ent->sess_freq) * (double)(UINT_MAX/2));
	htm_chord_expand(ret_hash, csum);


	{
		chord_t hash;
		memset(&hash, 0, sizeof(hash));
		htm_chord_expand(&hash, csum);
//fprintf(stderr, "DEBUG: TIMESTAMP HASH: %s\n", htm_chord_hex(&hash));
	}

}




void htm_exec_brane_init(entity_t *ent, brane_t *br)
{
	ent->control.birth_stamp = ztime_now();

}





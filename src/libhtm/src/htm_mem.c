

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

static event_t mem_cache_table[MAX_HTM_MEM_CACHE];

#if 0
uint64_t htm_mem_csum(brane_t *br, cell_t *cell)
{
}

uint64_t htm_mem_event(brane_t *br, cell_t *cell)
{
}

void htm_mem_forget(chord_t *hash)
{
	htm_mem_cache_remove(hash);
}

int htm_mem_recall(chord_t *hash)
{
	cell_t *cell;

	cell = htm_mem_cache_get(hash);
	if (!cell)
		return (-1);

	/* activate event. */
	htm_cell_recall(cell, hash);

	/* discard event */
	free(e);
	htm_mem_forget(hash);

	return (0);
}

/* Record an active cell event. */
int htm_mem_set(cell_t *cell, int mem_type, chord_t *hash)
{

	htm_mem_cache_put(hash, cell);
	htm_mem_store(e);

	return (0);
}

/* Replay pooled events which are then stored or erased.  */
int htm_mem_purge(void)
{
	/* hippocampus replays 5s in 150ms in compressed time sequence duration  */
	/* aproximating a shortest segment time of 30ms */
	cell_t *cell;

	cell = htm_mem_retrieve();
	if (!cell)
		return (-1);

	return (0);
}

/**
 * The hippocampus collects ongoing cell spikes in the neocortex.
 * Cache matches for a cell will result in all cells being involved being broadcasted their original registered cell hash.
 * "Declarative expliticit memories"
 * Emotional exponentially affects rate of memories aquired.
 */
#endif



#if 0

void htm_mem_cache_set(event_t *e)
{
	uint64_t cache_idx;

	e->stamp = htime_now();
	cache_idx = (uint64_t)(e->stamp * 25);
	cache_idx = (cache_idx % MAX_HTM_MEM_CACHE);
	mem_cache_table[cache_idx] = e;
}

int htm_mem_cache_unset(htime t)
{
	uint64_t cache_idx;

	cache_idx = (uint64_t)(t * 25);
	cache_idx = (cache_idx % MAX_HTM_MEM_CACHE);
	if (mem_cache_table[cache_idx])
		free(mem_cache_table[cache_idx]);
	mem_cache_table[cache_idx] = NULL;
}

event_t *htm_mem_cache_get(htime t)
{
	uint64_t cache_idx;

	cache_idx = (t * 25);
	cache_idx = (cache_idx % MAX_HTM_MEM_CACHE);
	return (mem_cache_table[cache_idx]);
}
#endif




event_t *htm_mem_event_alloc(void)
{
	uint64_t cache_idx;
	event_t *e;
	htime stamp;

	stamp = htime_now();
	cache_idx = (uint64_t)(stamp * 25);
	cache_idx = (cache_idx % MAX_HTM_MEM_CACHE);

	e = &mem_cache_table[cache_idx];
	memset(e, '\000', sizeof(event_t));
	e->stamp = stamp;

	return (e);
}

/* emulates an episodic buffer. */
event_t *htm_mem_set(entity_t *ent, cell_t *cell)
{
	layer_t *fe_l = &cell->layer[CELL_L3]; /* Feature (pool) */
	layer_t *obj_l = &cell->layer[CELL_L5a]; /* Object (pool) */
	layer_t *exp_l = &cell->layer[CELL_L1]; /* Experience (pool) */
	event_t *e;
	
/* todo: search for past mem w/ "exp" hash & over-ride when transferred to episodic memory */
	e = htm_mem_event_alloc();

	e->cell_id = cell->id;
	memcpy(&e->h_feat, &fe_l->hash, sizeof(chord_t)); 
	memcpy(&e->h_obj, &fe_l->hash, sizeof(chord_t)); 
	memcpy(&e->h_exp, &exp_l->hash, sizeof(chord_t)); 

	/* sum together emotional resonance of all experiences for object. */ 
	e->resonance += htm_emo_resonance(ent, &obj_l->hash);

	return (e);
}


/*
 * Semantic information is derived from accumulated episodic memory. Episodic memory can be thought of as a "map" that ties together items in semantic memory. For example, all encounters with how a "dog" looks and sounds will make up the semantic representation of that word. All episodic memories concerning a dog will then reference this single semantic representation of "dog" and, likewise, all new experiences with the dog will modify the single semantic representation of that dog.
 *
 * Episodic Memory:
 * * Contain summary records of sensory-perceptual-conceptual-affective processing.
 * * Retain patterns of activation/inhibition over long periods.
 * * Often represented in the form of (visual) images.
 * * They always have a perspective (field or observer).
 * * Represent short time slices of experience.
 * * They are represented on a temporal dimension roughly in order of occurrence.
 * * They are subject to rapid forgetting.
 * * They make autobiographical remembering specific.
 * * They are recollectively experienced when accessed.
 */



/*
 * Working Memory (short-term memory)
 * * limited capacity (~ seven objects)
 * * organizational encoding - single object representing quantities of objects (chunking)
 *
 */

/** Fill episodic memory buffer. */
/* When an event is experienced the working memory is recorded as an object. */
void htm_mem_notify(entity_t *ent, cell_t *cell)
{
	const int state = htm_control_state(ent);
	event_t *event;

	event = NULL;
	switch (state) {
		case STATE_IDLE:
			/* -> STATE_ATTENTION */
			break;
		case STATE_SLEEP:
		case STATE_RECALL:
		case STATE_ATTENTION:
			event = htm_mem_set(ent, cell);
			break;

		case STATE_FOCUS_POSITION: 
			{
				chord_t *focus_loc = htm_focused_location(ent);
				chord_t *cell_loc = htm_cell_location(cell);
				if (htm_chord_match(focus_loc, cell_loc))
					event = htm_mem_set(ent, cell);
			}
			break;

		case STATE_FOCUS_FEATURE:
			{
				chord_t *focus_fea = htm_focused_feature(ent);
				chord_t *cell_fea = htm_cell_feature(cell);
				if (htm_chord_match(focus_fea, cell_fea))
					event = htm_mem_set(ent, cell);
			}
			break;

		case STATE_FOCUS_OBJECT:
			{
				chord_t *focus_obj = htm_focused_object(ent);
				chord_t *cell_obj = htm_cell_object(cell);
				if (htm_chord_match(focus_obj, cell_obj))
					event = htm_mem_set(ent, cell);
			}
			break;
	}
	if (!event)
		return;

	/* generate operation to recall object associated with features (semantic). */
	{
		uint64_t key;
		int tot;
		int i;
fprintf(stderr, "DEBUG: MEM NOTIFY: feature %s\n", htm_chord_hex(&event->h_feat)); 

		tot = 0;
		for (i = 0; i < MAX_HTM_MEM_CACHE; i++) {
			if (!mem_cache_table[i].stamp)
				continue; /* not set */

fprintf(stderr, "DEBUG: MEM NOTIFY: feature compare %s\n", htm_chord_hex(&mem_cache_table[i].h_feat));
			if (!htm_chord_match(&event->h_feat, &mem_cache_table[i].h_feat))
				continue; /* not similar */

			/* similar feature */
			key = htm_chord_compact(&event->h_feat);
			zpu_push(&ent->zpu, ZPU_VAR, zpu_num64(key));
			tot++;
			if (tot >= MAX_HTM_MEM_MATCH)
				break;
		}
fprintf(stderr, "DEBUG: MEM NOTIFY: feature total x%d\n", tot);

		if (tot < MAX_HTM_MEM_MATCH)
			zpu_push(&ent->zpu, ZPU_BIT_OR, zpu_num32(tot)); /* chunk together */
		else
			zpu_push(&ent->zpu, ZPU_BIT_XOR, zpu_num32(tot)); /* separate patterns */
		zpu_push(&ent->zpu, ZPU_DB_RECALL, NULL);
	}

	/* generate operation to recall experience associated with features (episodic). */
	{
		uint64_t key;
		int tot;
		int i;

		tot = 0;
		for (i = 0; i < MAX_HTM_MEM_CACHE; i++) {
			if (!htm_chord_match(&mem_cache_table[i].h_exp, &event->h_feat))
				continue;

			/* similar feature */
			key = htm_chord_compact(&event->h_exp);
			zpu_push(&ent->zpu, ZPU_VAR, zpu_num64(key));
			tot++;
			if (tot >= MAX_HTM_MEM_MATCH)
				break;
		}

		/* only recall an experience when feature is unique. */
		if (tot < MAX_HTM_MEM_MATCH) {
			zpu_push(&ent->zpu, ZPU_BIT_OR, zpu_num32(tot)); /* chunk together */
			zpu_push(&ent->zpu, ZPU_DB_RECALL, NULL);
		}
	}

}

/** 
 * Read from pesistent storage.
 * @see ZPU_DB_RECALL
 */
void htm_mem_recall(zpu_t *z, uint64_t key)
{
	chord_t *obj;
	zvar nobj;

fprintf(stderr, "DEBUG: htm_mem_recall: key = %llu\n", (unsigned long long)key);

	/* match any similar keys in long-term episodic memory. */
	obj = htm_mem_restore(z, key); /* matches using layer cache logic; partial match */
	if (!obj)
		return;

	zpu_push(z, ZPU_VAR, zpu_num64(htm_chord_compact(obj)));
	zpu_push(z, ZPU_VAR, zpu_num64(key));
	zpu_push(z, ZPU_DB_STORE, NULL);
}

/**
 * Write to persistent storage 
 * @note When an event is recalled the original recorded events comprising it are re-notified.
 * @see ZPU_DB_STORE
 */
void htm_mem_remember(zpu_t *z, uint64_t key, uint64_t data)
{
	chord_t nobj;
	chord_t obj;
	int i;

/*
 * In the hippocampus, the EC level will send to both the dentate gyrus and the C1 section. When the C1 section receives input from the CA3 section (from dentate) that matches what it received from EC, it will send it back to the neocortex.
 *
 * This is emulated by utilizing the primary executive brane cell; which also naturally reflects focused items.
 */

	memset(&obj, 0, sizeof(obj));
	htm_chord_expand(&obj, data);

	memset(&nobj, 0, sizeof(chord_t));
	for (i = 0; i < MAX_HTM_MEM_CACHE; i++) {
		if (!htm_chord_match(&mem_cache_table[i].h_obj, &obj))
			continue;

		/* append similar object definition */
		htm_chord_merge(&nobj, &mem_cache_table[i].h_obj);
		/* append experience related to object. */
		htm_chord_merge(&nobj, &mem_cache_table[i].h_exp);
	}
	/* persitently store reference to object and experience. */
	htm_mem_store(z, key, &nobj);

	/* broadcast feedback to relevant branes as input */
	htm_mem_relay(z, &nobj); 
}

/*
 * Long term memory is composed of procedural, episodic, and semantic types.
 * Procedural memory is an unconcious collection of instructions for performing actions.
 *  - Memory of motor skills (riding a bike)
 * Semantic memory is a concious collection of relevant factual based information.
 *  - China is in asia.
 * Episodic memory is a concious collection of personal experiences.
 *  - Memory of your first kiss. 
 */

chord_t *htm_mem_restore(zpu_t *z, uint64_t key)
{
	/* .. */
	return (NULL);
}

void htm_mem_relay(zpu_t *z, chord_t *hash)
{
	/* .. */
}

void htm_mem_store(zpu_t *z, uint64_t key, chord_t *hash)
{
	/* .. */
}

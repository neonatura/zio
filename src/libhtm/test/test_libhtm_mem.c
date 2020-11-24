
#include "test_libhtm.h"


_TEST(mem)
{



#if 0
	brane_t *neo_br = htm_brane_get(test_entity, BR_NEO);
	zput_t *z = (zpu_t *)&neo_br->zpu;
	layer_t *obj_l;
	layer_t *feat_l;
	layer_t *exp_l;
	chord_t data;
	brane_t *br;
	cell_t *cell;
	zvar tvar;

	/* transition to state where all memories are stored. */
	htm_control_state_set(test_entity, STATE_ATTENTION);

	/* prepare a test cell */
	br = htm_brane_get(test_entity, BR_EXEC);
	_TRUEPTR(br);
	cell = htm_cell_init(br);
	_TRUEPTR(cell);

	obj_l = &cell->layer[CELL_L5a];
	feat_l = &cell->layer[CELL_L3];
	exp_l = &cell->layer[CELL_L1];

	memset(&data, '\002', sizeof(data));
	htm_layer_set(feat_l, &data);
	htm_layer_set(obj_l, &data);
	htm_layer_set(exp_l, &data);
	/* .. */

	/* initiate working memory restore/store sequence.*/
	htm_mem_notify(test_entity, cell);

	/* perform recall (semantic) */
fprintf(stderr, "DEBUG: TEST: recall/semantic start\n");
	_TRUE(z->sp != 0);
	zpu_exec(z);
//	_TRUE(z->sp == 0);
fprintf(stderr, "DEBUG: TEST: recall/semantic finish\n");

	/* perform recall (episodic) */
fprintf(stderr, "DEBUG: TEST: recall/episodic start\n");
	zpu_exec(z);
	_TRUE(z->sp == 0);
fprintf(stderr, "DEBUG: TEST: recall/episodic finish\n");

	/* perform remember */
fprintf(stderr, "DEBUG: TEST: remember start\n");
	zpu_exec(z);
	_TRUE(z->sp == 0);
fprintf(stderr, "DEBUG: TEST: remember finish\n");

#endif



}


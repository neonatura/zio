
#include "test_libhtm.h"

_TEST(quat)
{
  qnum tnum1;
  qnum tnum2;
  qvar tnum3;
  qvar tnum4;
  double tval1 = -111.111;
  double tval2 = 2220;
  double tval3;
  double tval4;
  tval3 = tval1 + tval2;
  tval4 = tval1 - tval2;


	quat_op_init();

	memset(tnum1, 0, sizeof(tnum1));
  quat_set(tval1, Q_NUM, tnum1);

	memset(tnum2, 0, sizeof(tnum2));
  quat_set(tval2, Q_NUM, tnum2);

  tnum3 = quat_op(QOP_MATH_SUM, tnum1, tnum2);
	_TRUE(quat_getf(tnum3) == (tval1 + tval2));

  tnum4 = quat_op(QOP_MATH_SUB, tnum1, tnum2);
	_TRUE(quat_getf(tnum4) == (tval1 - tval2));

}

_TEST(zpu)
{
  double tval1 = -111.111;
	double tval2 = 2220;
  qnum tnum1;
  qnum tnum2;
	zvar tvar;
	zpu_t *z;

	z = (zpu_t *)&test_entity->zpu;

	memset(tnum1, 0, sizeof(qnum));
	memset(tnum2, 0, sizeof(qnum));
	quat_set(tval1, Q_NUM, (qvar)tnum1);
	quat_set(tval2, Q_NUM, (qvar)tnum2);

	/* additive math operation */

	zpu_push(z, ZPU_VAR, tnum1);
	zpu_push(z, ZPU_VAR, tnum2);
	zpu_push(z, ZPU_MATH_SUM, NULL);
	zpu_exec(z);
	tvar = zpu_pop_var(z);
	_TRUE(quat_getf(tvar) == (tval2 + tval1)); 

	/* subtractive math operation */
	zpu_push(z, ZPU_VAR, tnum1);
	zpu_push(z, ZPU_VAR, tnum2);
	zpu_push(z, ZPU_MATH_SUB, NULL);
	zpu_exec(z);
	tvar = zpu_pop_var(z);
	_TRUE(quat_getf(tvar) == (tval2 - tval1)); 

}

_TEST(mem)
{
	zpu_t *z = (zpu_t *)&test_entity->zpu;
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
	_TRUE(z->top != 0);
	zpu_exec(z);
//	_TRUE(z->top == 0);
fprintf(stderr, "DEBUG: TEST: recall/semantic finish\n");

	/* perform recall (episodic) */
fprintf(stderr, "DEBUG: TEST: recall/episodic start\n");
	zpu_exec(z);
	_TRUE(z->top == 0);
fprintf(stderr, "DEBUG: TEST: recall/episodic finish\n");

	/* perform remember */
fprintf(stderr, "DEBUG: TEST: remember start\n");
	zpu_exec(z);
	_TRUE(z->top == 0);
fprintf(stderr, "DEBUG: TEST: remember finish\n");

}


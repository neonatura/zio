
#include "test_libquat.h"

_TEST(quat_math)
{
  qnum tnum1;
  qnum tnum2;
  qvar tnum3;
  qvar tnum4;
  qnum tnum5;
  qvar tnum6;
  double tval1 = -111.111;
  double tval2 = 2220;
  double tval3;
  double tval4;
  double tval5 = 9;
  tval3 = tval1 + tval2;
  tval4 = tval1 - tval2;


	quat_op_init();

	memset(tnum1, 0, sizeof(tnum1));
  quat_set(tval1, Q_NUM, tnum1);

	memset(tnum2, 0, sizeof(tnum2));
  quat_set(tval2, Q_NUM, tnum2);

	memset(tnum5, 0, sizeof(tnum5));
  quat_set(tval5, Q_NUM, tnum5);

  tnum3 = quat_op(QOP_MATH_SUM, tnum1, tnum2);
	_TRUE(quat_getf(tnum3) == (tval1 + tval2));

  tnum4 = quat_op(QOP_MATH_SUB, tnum1, tnum2);
	_TRUE(quat_getf(tnum4) == (tval1 - tval2));

	tnum6 = quat_op(QOP_MATH_SLL, tnum2, tnum5); 
	_TRUE((unsigned int)quat_get(tnum6) == ((unsigned int)tval2 << (unsigned int)tval5));


}


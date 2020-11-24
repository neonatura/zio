
#include "test_libquat.h"

_TEST(quat_math)
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


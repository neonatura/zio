
#include "test_libzpu.h"

_TEST(zpu_math)
{
	zprocessor_t *zproc = test_zproc;
	zpu_t *z = test_zpu;
	zreg_t *reg;
  double tval1 = -111.111;
	double tval2 = 1220;
  qnum tnum1;
  qnum tnum2;
	zvar tvar;

	memset(tnum1, 0, sizeof(qnum));
	memset(tnum2, 0, sizeof(qnum));
	quat_set(tval1, Q_NUM, (qvar)tnum1);
	quat_set(tval2, Q_NUM, (qvar)tnum2);

	/* startup */
//	zpu_exec(z);

#if 0
	/* bit: encode & decode */
	zpu_push(z, ZINST_ENC, tnum1, tnum2);
	zpu_exec(z);
	tvar = zpu_pop_var(z);
	zpu_push(z, ZINST_DEC, tvar, tnum2);
	zpu_exec(z);
	tvar = zpu_pop_var(z);
	_TRUE(quat_getf(tvar) == quat_getf(tnum1));
#endif

	/* math: addition */
	zpu_push(z, ZINST_ADD, ZREG_S1, ZREG_Z, ZREG_VAR, tnum1); 
	zpu_exec(test_zproc, z);
	zpu_push(z, ZINST_ADD, ZREG_S2, ZREG_S1, ZREG_VAR, tnum2); 
	zpu_exec(test_zproc, z);
	reg = zpu_reg_get(z, ZREG_S2);
	_TRUEPTR(reg);
	tvar = (zvar)reg->data; 
fprintf(stderr, "DEBUG: tvar = %f (t1+t2=%f)\n", (double)quat_get(tvar), (double)(tval1+tval2));
	_TRUE(quat_getf(tvar) == (tval1 + tval2)); 


	/* math: subtraction */
	zpu_push(z, ZINST_ADDI, ZREG_S1, ZREG_Z, 511);
	zpu_exec(zproc, z);
	zpu_push(z, ZINST_ADDI, ZREG_S2, ZREG_Z, 2047);
//	zpu_push(z, ZINST_SLLI, ZREG_T1, ZREG_T0, 12); /* T1 = 0x7FF000 */
	zpu_exec(zproc, z);
	zpu_push(z, ZINST_SUB, ZREG_S3, ZREG_S2, ZREG_S1);
	zpu_exec(zproc, z);

	reg = zpu_reg_get(z, ZREG_S1); 
fprintf(stderr, "DEBUG: ZPU_MATH: SUB: ZREG_S1 = %f\n", (double)quat_get(reg->data));
	reg = zpu_reg_get(z, ZREG_S2); 
fprintf(stderr, "DEBUG: ZPU_MATH: SUB: ZREG_S2 = %f\n", (double)quat_get(reg->data));
	reg = zpu_reg_get(z, ZREG_S3); 
fprintf(stderr, "DEBUG: ZPU_MATH: SUB: ZREG_S3 = %f\n", (double)quat_get(reg->data));
	_TRUE((double)quat_get(reg->data) == ((double)2047 - (double)511));

}



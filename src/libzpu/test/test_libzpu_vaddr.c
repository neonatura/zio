
#include "test_libzpu.h"

_TEST(zpu_vaddr)
{
#if 0
	zpu_t *z = (zpu_t *)test_zpu;
	zreg_t *reg;

	zpu_push(z, ZINST_ADDI, ZREG_T0, ZREG_Z, ZREG_IMM, 123);
	zpu_push(z, ZINST_ADDI, ZREG_T1, ZREG_Z, ZREG_IMM, 12345);
	zpu_push(z, ZINST_ADDI, ZREG_T2, ZREG_T1, ZREG_T2);
	zpu_push(z, ZINST_SW, ZREG_T2, ZREG_Z, ZREG_IMM, 536870912); 
	zpu_push(z, ZINST_LW, ZREG_T3, ZREG_Z, ZREG_IMM, 536870912);

	reg = zpu_reg_get(z, ZREG_T3);
	fprintf(stderr, "DEBUG: ZPU_VADDR: ZREG_T3 = %f\n", (double)quat_get(reg->data)); 
#endif
}


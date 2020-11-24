
#include "test_libzpu.h"

#define ZPU_BIOS_MEM_ADDR 0x0

_TEST(zpu_bios)
{
#if 0
	zpu_t *z = test_zpu;
	qnum var;
	uint32_t addr;
	int32_t imm;
	int rd;

	/* Interrupt Service Routine (ISR) */
	addr = z->sp;

	/* Interrupt Vector Table */
	zpu_push(z, ZINST_ADDI, ZREG_T1, ZREF_IMM, addr);
	zpu_push(z, SW, ZREF_T1, ZREF_IMM,  

	/* setup global pointer */
#if 0
	memset(var, 0, sizeof(var));
	quat_set(0x20800, Q_NUM, var);
	zpu_reg_set(z, ZREG_GP, (qvar)var); 
#endif
	zpu_push(z, ZINST_ADDI, ZREG_GP, ZREG_Z, ZREG_IMM, 0x20800);

	/* establish 'bios memory address tail' referencing next allocation location immediately after bios . */
	zpu_push(z, ZINST_ADDI, ZREG_T0, ZREG_Z, ZREG_IMM, 0x30000);
	zpu_push(z, ZINST_SW, ZREG_GP, ZREG_T0, NULL);

	/* alloc func */
	addr = z->sp;
	zpu_push(z, ZINST_LW, ZREG_T0, ZREG_GP, NULL);
	/* next address after this */
	zpu_push(z, ZINST_SW, ZREG_T0, ZREG_ZREG_IMM, 0xZREG_T0


	/* store "addr" in vector interrupt table */
#endif

}


/*
 *  Copyright 2020 Neo Natura
 *
 *  This file is part of the zio project.
 *  (https://github.com/neonatura/the zio project)
 *
 *  The zio project is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  The zio project is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with the zio project.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __ZPU_H__
#define __ZPU_H__

/* libzsys */
#include "zsys.h"

/* libquat */
#include "quat.h"

/* libzdb */
#include "zdb.h"

/* The "ZPU" module is used in order to provide a limited capacity microprocessor, which acts as the entity's working memory, and is used for problem solving and other computational purposes.  */

#define MAX_ZPU_STACK 16384

/* instruction parameter attribute */
#define ZATTR_LITERAL 0 /* literal number */
#define ZATTR_STACK 1 /* index on stack */
#define ZATTR_MEM 2 /* memory address */
#define ZATTR_PTR 3 /* mem addr of a mem addr */
#define ZATTR_REG 4 /* register index; 0 = accumulator */
#define ZATTR_VARG 5 /* variable number of args on stack */

/* zpu processing flags */
#define ZPUF_CARRY (1 << 0)
#define ZPUF_ZERO (1 << 1)
#define ZPUF_INTR_DISABLE (1 << 2)
#define ZPUF_BREAK (1 << 3)
#define ZPUF_OVERFLOW (1 << 4)
#define ZPUF_NEGATIVE (1 << 5)

/* categories of op codes */
#define ZINST_TYPE_LOAD 0x03   /* '0000011; load from memory cache */
#define ZINST_TYPE_SAVE 0x23   /* '0100011; save to memory cache */
#define ZINST_TYPE_BRANCH 0x63 /* '1100011; if-else */
#define ZINST_TYPE_ALCI 0x13   /* '0010011; arithmetic, logic, and compare (immediate) */
#define ZINST_TYPE_ALC 0x33    /* '0110011; arithmetic, logic, and compare (register) */
#define ZINST_TYPE_SYSTEM 0x73 /* '1110011; internal/diagnostic */
#define ZINST_TYPE_JAL 0x6F    /* '1101111: jump stack address */
#define ZINST_TYPE_JALR 0x67   /* '1100111: jump stack address */
#define ZINST_TYPE_BIT 0x0F    /* '0001111: extended bit operations. */
#define ZINST_TYPE_MEM 0x17    /* '0010111: memory segment operations. */

/* an operation that performs no action. */
#define ZINST_NOP 0x00
/* a global-scope variable */
#define ZINST_GLO 0x01
/* an instruction referencing a local variable. */
#define ZINST_VAR 0x02
/* an instruction referencing a in-mutable local literal */
#define ZINST_CON 0x03
/* a variable referenced in a memory address */
#define ZINST_REF 0x04
/* an unconditionally zero value */
#define ZINST_FAL 0x05
/* an unconditionally positive value */
#define ZINST_TRU 0x06
/* an undefined value */
#define ZINST_NUL 0x07

#if 0
/* copy a stack index instruction to another location. */
#define ZINST_MOV 0x10
/* copy a memory address value to another location. */
#define ZINST_MOVM 0x11
/* duplicate the next instruction on the stack. */
#define ZINST_DUP 0x14
/* load single value from mem addr referenced by <param> to stack. */
#define ZINST_LDS 0x15
/* write single value from stack to mem addr referenced by <param>. */
#define ZINST_LDM 0x16
#endif

/* load from memory cache operations */
#define ZINST_LB 0x30  /* 0000011:0000 */
#define ZINST_LH 0x31  /* 0000011:0001 */
#define ZINST_LW 0x32  /* 0000011:0010 */
#define ZINST_LD 0x33  /* 0000011:0011 */
#define ZINST_LBU 0x34 /* 0000011:0100 */
#define ZINST_LHU 0x35 /* 0000011:0101 */
#define ZINST_LWU 0x36 /* 0000011:0110 */

/* write to memory cache operations */
#define ZINST_SB 0x230  /* 0100011:0000 */
#define ZINST_SH 0x231  /* 0100011:0001 */
#define ZINST_SW 0x232  /* 0100011:0010 */
#define ZINST_SD 0x233  /* 0100011:0011 */

/* branch comparison ops */
#define ZINST_BEQ 0x630  /* '1100011:0000 */
#define ZINST_BNE 0x631  /* '1100011:0001 */ 
#define ZINST_BLT 0x634  /* '1100011:0100 */
#define ZINST_BGE 0x635  /* '1100011:0101 */
#define ZINST_BLTU 0x636 /* '1100011:0110 */
#define ZINST_BGEU 0x637 /* '1100011:0111 */

#if 0
/* Sets the program counter to the address specified by the parameter. */
#define ZINST_JMP 0x30
/* Jump to sub-routine; The JSR instruction pushes the address (minus one) of the return point on to the stack and then sets the program counter to the target memory address.. */
#define ZINST_JSR 0x31
/* The RET instruction is used at the end of a subroutine to return to the calling routine. It pulls the program counter (minus one) from the stack. */
#define ZINST_RET 0x32
#endif

/* immediate ALC */
#define ZINST_ADDI   0x4C00  /* '0010011:000:0000000 */
#define ZINST_SUBI   0x4C20  /* '0010011:000:0100000 */
#define ZINST_SLLI   0x4C80  /* '0010011:001:0000000 */
#define ZINST_SLTI   0x4D00  /* '0010011:010:0000000 */
#define ZINST_SLTUI  0x4D80  /* '0010011:011:0000000 */
#define ZINST_XORI   0x4E00  /* '0010011:100:0000000 */
#define ZINST_SRLI   0x4E80  /* '0010011:101:0000000 */
#define ZINST_SRAI   0x4EA0  /* '0010011:101:0100000 */
#define ZINST_ORI    0x4F00  /* '0010011:110:0000000 */
#define ZINST_ANDI   0x4F80  /* '0010011:111:0000000 */

/* register ALC */
#define ZINST_ADD    0xCC00  /* '0110011:000:0000000 */
#define ZINST_SUB    0xCC20  /* '0110011:000:0100000 */
#define ZINST_SLL    0xCC80  /* '0110011:001:0000000 */
#define ZINST_SLT    0xCD00  /* '0110011:010:0000000 */
#define ZINST_SLTU   0xCD80  /* '0110011:011:0000000 */
#define ZINST_XOR    0xCE00  /* '0110011:100:0000000 */
#define ZINST_SRL    0xCE80  /* '0110011:101:0000000 */
#define ZINST_SRA    0xCEA0  /* '0110011:101:0100000 */
#define ZINST_OR     0xCF00  /* '0110011:110:0000000 */
#define ZINST_AND    0xCF80  /* '0110011:111:0000000 */

/* stack [relative] address jump operations */
#define ZINST_JAL 0x6F0   /* '1101111:0000 */
#define ZINST_JALR 0x670  /* '1100111:0000 */

/* Encode a value using a key. */
#define ZINST_ENC 0x3E8
/* Decode a value using a key. */
#define ZINST_DEC 0x3F2 
/* bit: Hash two values together. */
#define ZINST_HAS 0x41A

/* mem: concat */
#define ZINST_CAT 0x4B0
/* mem: tail */
#define ZINST_TAI 0x4BA
/* mem: head */
#define ZINST_HEA 0x4C4
/* mem: soundex */
#define ZINST_SDX 0x4CE

/* entity: recall an event in working memory. */
#define ZINST_REC 0x898
/* entity: remember an event from working memory. */
#define ZINST_REM 0x899

#define MAX_ZPU_REGISTER_SIZE 64

#define ZREG_Z 0 /* Zero */
#define ZREG_RA 1 /* Return address */
#define ZREG_SP 2 /* Stack pointer */
#define ZREG_GP 3 /* Global pointer */
#define ZREG_TP 4 /* Thread pointer */
#define ZREG_T0 5 /* temp 0 / alternate return address */
#define ZREG_T1 6 /* temporary */
#define ZREG_T2 7 /* temporary */
#define ZREG_S0 8 /* saved register / frame pointer */
#define ZREG_FP ZREG_S0
#define ZREG_S1 9 /* saved register */
#define ZREG_A0 10 /* function arg / return value */
#define ZREG_A1 11 /* function arg / return value */
#define ZREG_A2 12 /* function arg */
#define ZREG_A3 13 /* function arg */
#define ZREG_A4 14 /* function arg */
#define ZREG_A5 15 /* function arg */
#define ZREG_A6 16 /* function arg */
#define ZREG_A7 17 /* function arg */
#define ZREG_S2 18 /* saved register */
#define ZREG_S3 19 /* saved register */
#define ZREG_S4 20 /* saved register */
#define ZREG_S5 21 /* saved register */
#define ZREG_S6 22 /* saved register */
#define ZREG_S7 23 /* saved register */
#define ZREG_S8 24 /* saved register */
#define ZREG_S9 25 /* saved register */
#define ZREG_S10 26 /* saved register */
#define ZREG_S11 27 /* saved register */
#define ZREG_T3 28 /* temporary */
#define ZREG_T4 29 /* temporary */
#define ZREG_T5 30 /* temporary */
#define ZREG_T6 31 /* temporary */

/* special codes */
#define ZREG_VAR (int64_t)(-1)

#define ZPU_FAMILY_R 1
#define ZPU_FAMILY_I 2
#define ZPU_FAMILY_S 3
#define ZPU_FAMILY_B 4
#define ZPU_FAMILY_U 5 
#define ZPU_FAMILY_J 6

#define DEFAULT_ZPU_FAMILY ZPU_FAMILY_R

#define ZINST_TYPE_SHIFT 10
#define ZINST_OP_SHIFT 7

#define ZINST_FAMILY(_opcode) \
	( \
		( \
			(_opcode) == ZINST_JAL || \
			(_opcode) == ZINST_JALR \
		) ? ZPU_FAMILY_J : \
		( \
			(_opcode) == ZINST_BEQ || \
			(_opcode) == ZINST_BNE || \
			(_opcode) == ZINST_BLT || \
			(_opcode) == ZINST_BGE || \
			(_opcode) == ZINST_BLTU || \
			(_opcode) == ZINST_BGEU \
		) ? ZPU_FAMILY_B : \
		( \
			(_opcode) == ZINST_LB || \
			(_opcode) == ZINST_LH || \
			(_opcode) == ZINST_LW || \
			(_opcode) == ZINST_LD || \
			(_opcode) == ZINST_LBU || \
			(_opcode) == ZINST_LHU || \
			(_opcode) == ZINST_LWU ||\
			(_opcode) == ZINST_ADDI || \
			(_opcode) == ZINST_SLTI || \
			(_opcode) == ZINST_XORI || \
			(_opcode) == ZINST_ORI || \
			(_opcode) == ZINST_ANDI || \
			(_opcode) == ZINST_SLLI || \
			(_opcode) == ZINST_SRLI || \
			(_opcode) == ZINST_SRAI \
		) ? ZPU_FAMILY_I : \
		( \
			(_opcode) == ZINST_SB || \
			(_opcode) == ZINST_SH || \
			(_opcode) == ZINST_SW || \
			(_opcode) == ZINST_SD \
		) ? ZPU_FAMILY_S : \
		( \
			(_opcode) == ZINST_ADD || \
			(_opcode) == ZINST_SLT || \
			(_opcode) == ZINST_XOR || \
			(_opcode) == ZINST_OR || \
			(_opcode) == ZINST_AND || \
			(_opcode) == ZINST_SLL || \
			(_opcode) == ZINST_SRL || \
			(_opcode) == ZINST_SRA \
		) ? ZPU_FAMILY_R : DEFAULT_ZPU_FAMILY \
	)

#define ZINST_TYPE(_opcode) \
  ((_opcode) >> ZINST_TYPE_SHIFT)

#define ZINST_OP(_opcode) \
  (((_opcode) >> ZINST_OP_SHIFT) & 7)

#define ZINST_SUBOP(_opcode) \
	( \
		(_opcode) == ZINST_SRAI || \
		(_opcode) == ZINST_SUB || \
		(_opcode) == ZINST_SRA \
	) ?  ((_opcode) & 127) : 0; 

/*
	 OP_NEWTABLE
	 OPT_SETUPVAL
	 SETTABLE
	 OP_POW
	 OP_MOD
	 OP_LEN
	 OPT_TEST
	 OP_FORLOOP
*/

typedef uint64_t zaddr_t;

typedef qvar zvar;

typedef struct zreg_t {
  uint32_t flag;
  uint32_t size;
  uint32_t refs;
  uint32_t fd;
  uint8_t *data;
} zreg_t;

typedef struct zinst_t {
  uint32_t type:7;
  uint32_t rdest:5;
  uint32_t op:3;
  uint32_t src1:5;
  uint32_t src2:5;
  uint32_t subop:5;
} zinst_t;

typedef struct zinstU_t {
  uint32_t type:7;
  uint32_t rdest:5;
  int32_t addr:20;
} zinstU_t;

typedef struct zinstI_t {
  uint32_t type:7;
  uint32_t rdest:5;
  uint32_t op:3;
  uint32_t src1:5;
  int32_t addr:12;
} zinstI_t;

typedef struct zpu_t {
  zreg_t *reg;
  zdb_t *page_table[65536];
  zinst_t inst[MAX_ZPU_STACK];
	uint64_t addr_logical_max;
  uint32_t reg_size;
  uint32_t pc; /* program counter; index of next stack execution. */
  uint32_t sp; /* stack pointer; top of stack. */
  uint32_t flag; /* cpu flags */
} zpu_t;

#include "zpu_exec.h"
#include "zpu_init.h"
#include "zpu_vaddr.h"
#include "zpu_vaddr_rom.h"
#include "zpu_opcode.h"
#include "zpu_reg.h"

#endif /* ndef __ZPU_H__ */

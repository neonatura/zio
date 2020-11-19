
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

#include "zpu.h"

#define REV_I32 "I32"
#define REV_I64 "I64"
struct inst_table_s {
	const char *name;
	int code;
	const char *rev;
};
static struct inst_table_s _inst_table[] = 
{
	{ "NOP", ZINST_NOP, REV_I32 },

	{ "LB", ZINST_LB, REV_I32 },
	{ "LH", ZINST_LH, REV_I32 },
	{ "LW", ZINST_LW, REV_I32 },
	{ "LD", ZINST_LD, REV_I64 },
	{ "LBU", ZINST_LBU, REV_I32 },
	{ "LHU", ZINST_LHU, REV_I32 },
	{ "LWU", ZINST_LWU, REV_I32 },

	{ "SB", ZINST_SB, REV_I32 },
	{ "SH", ZINST_SH, REV_I32 },
	{ "SW", ZINST_SW, REV_I32 },
	{ "SD", ZINST_SD, REV_I32 },

	{ "BEQ", ZINST_BEQ, REV_I32 },
	{ "BNE", ZINST_BNE, REV_I32 },
	{ "BLT", ZINST_BLT, REV_I32 },
	{ "BGE", ZINST_BGE, REV_I32 },
	{ "BLTU", ZINST_BLTU, REV_I32 },
	{ "BGEU", ZINST_BGEU, REV_I32 },

	{ "ADDI", ZINST_ADDI, REV_I32 },
	{ "SUBI", ZINST_SUBI, REV_I32 },
	{ "SLLI", ZINST_SLLI, REV_I32 },
	{ "SLTI", ZINST_SLTI, REV_I32 },
	{ "XORI", ZINST_XORI, REV_I32 },
	{ "SRLI", ZINST_SRLI, REV_I32 },
	{ "SRAI", ZINST_SRAI, REV_I32 },
	{ "ORI", ZINST_ORI, REV_I32 },
	{ "ANDI", ZINST_ANDI, REV_I32 },

	{ "ADD", ZINST_ADD, REV_I32 },
	{ "SUB", ZINST_SUB, REV_I32 },
	{ "SLL", ZINST_SLL, REV_I32 },
	{ "SLT", ZINST_SLT, REV_I32 },
	{ "XOR", ZINST_XOR, REV_I32 },
	{ "SRL", ZINST_SRL, REV_I32 },
	{ "SRA", ZINST_SRA, REV_I32 },
	{ "OR", ZINST_OR, REV_I32 },
	{ "AND", ZINST_AND, REV_I32 },

#if 0
	{ "GLO", ZINST_GLO, 2 },
	{ "VAR", ZINST_VAR, 1 },
	{ "CON", ZINST_CON, 1 },
	{ "REF", ZINST_REF, 1 },
	{ "FAL", ZINST_FAL, 0 },
	{ "TRU", ZINST_TRU, 0 },
	{ "NUl", ZINST_NUL, 0 },

	{ "MOV", ZINST_MOV, MAX_ZPU_ARGS },
	{ "DUP", ZINST_DUP, 0 },
	{ "LDS", ZINST_LDS, 1 },
	{ "LDM", ZINST_LDM, 2 },

	{ "JMP", ZINST_JMP, 1 },
	{ "JSR", ZINST_JSR, 1 },
	{ "RET", ZINST_RET, 1 },

	{ "BEQ", ZINST_BEQ, MAX_ZPU_ARGS },
	{ "BNE", ZINST_BNE, MAX_ZPU_ARGS },
	{ "BLT", ZINST_BLT, MAX_ZPU_ARGS },
	{ "BGE", ZINST_BGE, MAX_ZPU_ARGS },
	{ "BLTU", ZINST_BLTU, MAX_ZPU_ARGS },
	{ "BGEU", ZINST_BGEU, MAX_ZPU_ARGS },

	/* quat bit ops */
	{ "ENC", ZINST_ENC, 2 }, /* { value key } */ 
	{ "DEC", ZINST_DEC, 2 }, /* { value, key } */ 
	{ "IOR", ZINST_IOR, MAX_ZPU_ARGS },
	{ "XOR", ZINST_XOR, MAX_ZPU_ARGS },
	{ "AND", ZINST_AND, MAX_ZPU_ARGS },
	{ "HAS", ZINST_HAS, MAX_ZPU_ARGS },

	/* quat math ops */
	{ "SUM", ZINST_SUM, MAX_ZPU_ARGS }, /* { value + .. } */
	{ "SUB", ZINST_SUB, MAX_ZPU_ARGS }, /* { value - .. } */
	{ "MUL", ZINST_MUL, MAX_ZPU_ARGS },
	{ "DIV", ZINST_DIV, MAX_ZPU_ARGS },
	{ "EXP", ZINST_EXP, MAX_ZPU_ARGS },

	{ "CAT", ZINST_CAT, MAX_ZPU_ARGS },
	{ "TAI", ZINST_TAI, MAX_ZPU_ARGS },
	{ "HEA", ZINST_HEA, MAX_ZPU_ARGS },
	{ "SDX", ZINST_SDX, MAX_ZPU_ARGS },

	{ "LOA", ZINST_LOA, 2 },
	{ "SAV", ZINST_SAV, 2 },

	{ "SET", ZINST_SET, 2 },
	{ "GET", ZINST_GET, 2 },

	{ "REC", ZINST_REC, 2 },
	{ "REM", ZINST_REM, 1 },

	{ "CAT", ZINST_CAT, MAX_ZPU_ARGS },
#endif

	/* terminator */
	{ NULL }
};

const char *zpu_opcode_label(int code)
{
	static char buf[16];
	int i;

	memset(buf, 0, sizeof(buf));

	for (i = 0; _inst_table[i].name; i++) {
		if (_inst_table[i].code == code) {
			strncpy(buf, _inst_table[i].name, sizeof(buf));
			break;
		}
	}	

	if (!*buf) {
		sprintf(buf, "%u", (unsigned int)code);
	}

	return (buf);
}


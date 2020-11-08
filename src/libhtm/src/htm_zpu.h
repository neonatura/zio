
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

#ifndef __HTM_ZPU_H__
#define __HTM_ZPU_H__

#include "htm.h"

/**
 * The "ZPU" module is used in order to provide a limited capacity microprocessor which acts as the entity's working memory.
 *
 * The module provides the entity with a method to perform problem solving on temporary-based information.
 *
 * Templates for functions (quat streams) can be copied onto the stack from longer term memory. 
 */

/* internal */
#define ZPU_NULL 0
#define ZPU_VAR 70
#define ZPU_REF 71
/* comparator operatives */
#define ZPU_RETURN 80 
#define ZPU_EQUAL 81
#define ZPU_SOUNDEX 82
/* db operations */
#define ZPU_DB_RECALL 90
#define ZPU_DB_STORE 91
/* bit operations */
#define ZPU_BIT_ENCODE QOP_BIT_ENCODE
#define ZPU_BIT_DECODE QOP_BIT_DECODE
#define ZPU_BIT_OR QOP_BIT_OR
#define ZPU_BIT_XOR QOP_BIT_XOR
/* math operations */
#define ZPU_MATH_SUM QOP_MATH_SUM
#define ZPU_MATH_SUB QOP_MATH_SUB
#define ZPU_MATH_MULT QOP_MATH_MULT
#define ZPU_MATH_DIV QOP_MATH_DIV
#define ZPU_MATH_EXP QOP_MATH_EXP
/* mem/str operations */
#define ZPU_MEM_CONCAT QOP_MEM_CONCAT
#define ZPU_MEM_TAIL QOP_MEM_TAIL
#define ZPU_MEM_HEAD QOP_MEM_HEAD

#define ZNUM(_a) quat_getf(_a)
#define ZNUM32(_a) quat_get32(_b)
#define SET_ZNUM(_a, _b) quat_set(_b, Q_NUM, _a)
#define SET_ZNUM32(_a, _b) quat_set(_b, Q_NUM32, _a)

/** Add an instruction to the ZPU stack. */
uint32_t zpu_push(zpu_t *z, uint32_t inst, zvar param);

/** Retrieve variable on the top of the stack and remove instruction from stack. */
zinst_t *zpu_pop(zpu_t *z);

qvar zpu_pop_var(zpu_t *z);

/** Retrieve a specific variable on the stack. */
zinst_t *zpu_pull(zpu_t *z, int stack_index);

void zpu_exec(zpu_t *z);

zvar zpu_num32(uint32_t val);

zvar zpu_num64(uint64_t val);

zvar zpu_stream(void *data, size_t data_len);

#endif /* ndef __HTM_ZPU_H__ */


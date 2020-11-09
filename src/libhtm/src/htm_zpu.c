
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

#include "htm.h"

#define MAX_ZPU_STACK 256
static uint8_t _zpu_stack_instruction[MAX_ZPU_STACK];
static zvar _zpu_stack_param[MAX_ZPU_STACK];
static uint32_t _zpu_top;

uint32_t zpu_push(zpu_t *z, uint32_t inst, zvar param)
{
	const int idx = (++z->top);

	z->inst[idx].code = inst;
	z->inst[idx].param = param;

fprintf(stderr, "DEBUG: zpu_push: PUSH [#%d] OP %s (%f)\n", idx, zpu_code_label(inst), quat_getf(param));

	return (idx);
}

zinst_t *zpu_pull(zpu_t *z, int stack_index)
{
	return (&z->inst[stack_index]);
}

zinst_t *zpu_pop(zpu_t *z)
{
	uint32_t idx = z->top;

	if (idx == 0)
		return (NULL);

	z->top--;
	return (&z->inst[idx]);
}

#if 0
zinst_t *zpu_pop(zpu_t *z)
{
	zinst_t *inst;

	while (z->top) {
		inst = zpu_pull(z, z->top--);
		if (!inst)
			break;

		if (inst->code == ZPU_VAR)
			return (inst);

		/* recursive */
		zpu_exec(z);
	}

	return (NULL);
}
/*
{
	const int idx = z->top;
	
	if (idx == 0)
		return (NULL);

	z->top--;
	return (zpu_pull(z, idx));
}
*/
#endif

qvar zpu_pop_var(zpu_t *z)
{
	zinst_t *inst = zpu_pop(z);
	if (!inst) return (NULL);
	return (inst->param);
}
#if 0
/*
{
	const int idx = z->top;

	while (z->top) {
		zinst_t *inst = zpu_pop(z->top);
		if (!inst)
			break;
		if (inst->code == ZPU_VAR)
			return (
			return (inst->param);

		idx--;
	}

	if (idx && z->inst[idx].code == ZPU_VAR) {
		zinst_t *inst = zpu_pop(z);
		if (inst)
			return (inst->param);
	}

	return (NULL);
}
*/
#endif

zvar zpu_exec_arg(zpu_t *z)
{
	zinst_t *inst;
	zvar result;

	result = NULL;
	while (z->top) {
		inst = zpu_pull(z, z->top);
		if (!inst) {
			break;
		}

		if (inst->code != ZPU_NULL &&
				inst->code != ZPU_TRUE &&
				inst->code != ZPU_FALSE &&
				inst->code != ZPU_VAR) {
			zpu_exec(z);
			continue;
		}

		if (inst->code == ZPU_VAR)
			result = inst->param;
		else if (inst->code == ZPU_TRUE)
			result = zpu_num(~0);
		else if (inst->code == ZPU_FALSE)
			result = zpu_num(0);
fprintf(stderr, "DEBUG: zpu_exec_arg: [#%d] EXEC ARG: OP %s (%f)\n", z->top, zpu_code_label(inst->code), quat_getf(result));

		if (z->top > 0) z->top--;
		break;
	}

	return (result);
}

void zpu_exec(zpu_t *z)
{
	zinst_t *inst;

	inst = zpu_pop(z);
	if (!inst)
		return;


	if (inst->code == ZPU_VAR ||
			inst->code == ZPU_NULL)
		return; /* nothing to do */

fprintf(stderr, "DEBUG: zpu_exec: [#%d] EXEC (OP %s:%x)\n", z->top+1, zpu_code_label(inst->code), inst->param);

	switch (inst->code) {
		case ZPU_ENT_RECALL:
			{
				zvar kvar = zpu_exec_arg(z);
				chord_t *obj = htm_mem_recall(z, (uint64_t)quat_get(kvar));

				if (obj) {
					zpu_push(z, ZPU_VAR, zpu_num(htm_chord_compact(obj)));
					zpu_push(z, ZPU_ENT_REMEMBER, NULL);
					zpu_push(z, ZPU_VAR, kvar);
					zpu_push(z, ZPU_DB_SET, NULL);
				}

fprintf(stderr, "DEBUG: zpu_exec: ZPU_DB_RECALL: key %llu [obj: %x]\n", (unsigned long long)quat_get(kvar), obj);
			}
			break;

		case ZPU_ENT_REMEMBER:
			{
				zvar pvar = zpu_exec_arg(z);
				chord_t *obj = htm_mem_remember(z, (uint64_t)quat_get(pvar));
				uint64_t key = htm_chord_compact(obj);
				zpu_push(z, ZPU_VAR, zpu_num(key));
			}
			break;

		case ZPU_DB_GET:
			{
				zvar var1 = zpu_exec_arg(z);
				chord_t *obj = htm_mem_restore(z, (uint64_t)quat_get(var1));
				uint64_t key = htm_chord_compact(obj);
fprintf(stderr, "DEBUG: zpu_exec: ZPU_DB_GET: key %llu\n", (unsigned long long)quat_get(var1));
				zpu_push(z, ZPU_VAR, zpu_num(key));
			}
			break;

		case ZPU_DB_SET:
			{
				zvar kvar = zpu_exec_arg(z);
				zvar pvar = zpu_exec_arg(z);
				uint64_t key = (uint64_t)quat_get(kvar);
				chord_t hash;

				if (key != 0) {
					memset(&hash, 0, sizeof(hash));
					htm_chord_expand(&hash, (uint64_t)quat_get(pvar)); 
					htm_mem_store(z, key, &hash);
				}
				/* nothing returned.. */
			}
			break;

		default:
			{
				uint32_t args;
				qvar result = NULL;
				qvar var;
				int nr;

				if (inst->param == NULL) {
					args = 2;
				} else {
					args = (uint32_t)quat_get(inst->param);
				}

				for (nr = 0; nr < args; nr++) {
					var = zpu_exec_arg(z);

					if (nr == 0)
						result = var;
					else
						result = quat_op(inst->code, result, var);
				}
				zpu_push(z, ZPU_VAR, result);
//fprintf(stderr, "DEBUG: zpu_exec: default case result %f [args: %d]\n", (double)quat_get(result), args);
			}
			break;
	}

}

zvar zpu_num(uint64_t val)
{
	num_t r_val = (num_t)val;
	zvar ret_var = (zvar)quat_alloc(Q_NUM, &r_val, sizeof(num_t)); 
fprintf(stderr, "DEBUG: ZPU_NUM: ret_var = %llu\n", (unsigned long long)quat_get(ret_var));
//	return ((zvar)quat_alloc(Q_NUM, &r_val, sizeof(num_t))); 
	return (ret_var);
}

#if 0
zvar zpu_stream(void *data, size_t data_len)
{
	qvar var;
	int flag;

	var = (uint8_t *)calloc(data_len, sizeof(uint8_t));
	if (!var)
		return (NULL);

	memcpy(var, data, data_len);
	flag = quat_var_flag(var) | Q_STREAM;
	quat_var_flag_set(var, flag);

	return (var);
}
#endif

const char *zpu_code_label(int code)
{
	static char buf[16];

	switch (code) {
		case ZPU_NULL: return "NULL";
		case ZPU_TRUE: return "TRUE";
		case ZPU_FALSE: return "FALSE";
		case ZPU_VAR: return "VAR";
		case ZPU_REF: return "REF";
		case ZPU_DUP: return "DUP";
		case ZPU_LET: return "LET";
		case ZPU_RETURN: return "RETURN";
		case ZPU_EQUAL: return "EQUAL";
		case ZPU_EQUAL_SUB: return "EQUAL_SUB";
		case ZPU_EQUAL_SIM: return "EQUAL_SIM";
		case ZPU_EQUAL_NOT: return "EQUAL_NOT";
		case ZPU_DB_GET: return "GET";
		case ZPU_DB_SET: return "SET";
		case ZPU_ENT_RECALL: return "RECALL";
		case ZPU_ENT_REMEMBER: return "REMEMBER";
		case ZPU_BIT_ENCODE: return "ENCODE";
		case ZPU_BIT_DECODE: return "DECODE";
		case ZPU_BIT_OR: return "OR";
		case ZPU_BIT_XOR: return "XOR";
		case ZPU_BIT_AND: return "AND";
		case ZPU_BIT_HASH: return "HASH";
		case ZPU_MATH_SUM: return "SUM";
		case ZPU_MATH_SUB: return "SUB";
		case ZPU_MATH_MULT: return "MULT";
		case ZPU_MATH_DIV: return "DIV";
		case ZPU_MATH_EXP: return "EXP";
		case ZPU_MEM_CONCAT: return "CONCAT";
		case ZPU_MEM_TAIL: return "TAIL";
		case ZPU_MEM_HEAD: return "HEAD";
		case ZPU_MEM_SOUNDEX: return "SOUNDEX";
	}

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%u", (unsigned int)code);
	return (buf);
}


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

	return (idx);
}

zinst_t *zpu_pull(zpu_t *z, int stack_index)
{
	return (&z->inst[stack_index]);
}

zinst_t *zpu_pop(zpu_t *z)
{
	const int idx = z->top;

	if (!idx)
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

fprintf(stderr, "DEBUG: zpu_exec_arg: inst->code %d\n", inst->code);
		if (inst->code != ZPU_NULL &&
				inst->code != ZPU_VAR) {
			zpu_exec(z);
			continue;
		}

		if (inst->code == ZPU_VAR)
			result = inst->param;

		z->top--;
		break;
	}

	return (result);
}

void zpu_exec(zpu_t *z)
{
	zinst_t *inst;

fprintf(stderr, "DEBUG: ZPU EXEC: STACK x%d\n", z->top);

	inst = zpu_pop(z);
	if (!inst)
		return;


	if (inst->code == ZPU_VAR ||
			inst->code == ZPU_NULL)
		return; /* nothing to do */

fprintf(stderr, "DEBUG: zpu_exec: instruction {%d:%x} [stack: %d]\n", inst->code, inst->param, z->top);

	switch (inst->code) {
		case ZPU_DB_RECALL:
			{
				zvar var1 = zpu_exec_arg(z);
fprintf(stderr, "DEBUG: zpu_exec: ZPU_DB_RECALL: key %llu\n", (unsigned long long)quat_get(var1));
				htm_mem_recall(z, (uint64_t)quat_get(var1));
			}
			break;

		case ZPU_DB_STORE:
			{
				zvar kvar = zpu_exec_arg(z);
				zvar pvar = zpu_exec_arg(z);
				htm_mem_remember(z, (uint64_t)quat_get(kvar), (uint64_t)quat_get(pvar));
			}
			break;

		default:
			{
				int args;
				qvar result = NULL;
				qvar var;
				int nr;

				if (inst->param == NULL) {
					args = 2;
				} else {
					args = (int)quat_get32(inst->param);
				}

				for (nr = 0; nr < args; nr++) {
					var = zpu_exec_arg(z);

					if (nr == 0)
						result = var;
					else
						result = quat_op(inst->code, result, var);
				}
				zpu_push(z, ZPU_VAR, result);
fprintf(stderr, "DEBUG: zpu_exec: default case result %f [args: %d]\n", (double)quat_get(result), args);
			}
			break;
	}

}

zvar zpu_num32(uint32_t val)
{
	qnum tvar;

	quat_set32((num_t)val, Q_NUM, tvar);
	return (zpu_stream(tvar, sizeof(qnum)));
}

zvar zpu_num64(uint64_t val)
{
	qnum tvar;

	quat_set64((num_t)val, Q_NUM, tvar);
	return (zpu_stream(tvar, sizeof(qnum)));
}

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


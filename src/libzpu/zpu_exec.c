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

#define SIGN_EXTEND(_value) \
	  (int64_t)(_value)

/**
 * Loads a single variable from memory.
 */
int zpu_exec_load(zpu_t *z, uint32_t opcode, zaddr_t addr, zreg_t *rdest)
{
	qvar data;
	size_t data_len;
	qvar result;
	qnum tnum;
	int err;

	err = zpu_vaddr_get(z, addr, &data, &data_len);
	if (err)
		return (err);

	result = (qvar)tnum;
	switch (opcode) {
		case ZINST_LB:
			{
				int8_t v = (int8_t)quat_get(data);
				zpu_reg_nset(z, rdest, (num_t)v); 
			}
			break;
		case ZINST_LBU:
			{
				uint8_t v = (uint8_t)quat_get(data);
				zpu_reg_nset(z, rdest, (num_t)v); 
			}
			break;
		case ZINST_LH:
			{
				int16_t v = (int16_t)quat_get(data);
				zpu_reg_nset(z, rdest, (num_t)v); 
			}
			break;
		case ZINST_LHU:
			{
				uint16_t v = (uint16_t)quat_get(data);
				zpu_reg_nset(z, rdest, (num_t)v); 
			}
			break;
		case ZINST_LW:
			{
				int32_t v = (int32_t)quat_get(data);
				zpu_reg_nset(z, rdest, (num_t)v); 
			}
			break;
		case ZINST_LWU:
			{
				uint32_t v = (uint32_t)quat_get(data);
				zpu_reg_nset(z, rdest, (num_t)v); 
			}
			break;
		default:
			zpu_reg_set(z, rdest, data);
			break;
	}

	return (0);
}

void zpu_exec_save(zpu_t *z, uint32_t opcode, off_t addr, zreg_t *reg)
{
	zpu_vaddr_set(z, addr, (uint8_t *)reg->data, quat_var_size(reg->data));
}

/* compare values and branch to relative stack address if operator resolves to true. */
void zpu_exec_branch(zpu_t *z, uint32_t opcode, off_t addr, zreg_t *r1, zreg_t *r2)
{
	num_t result;
	int o_true;

	result = quat_get(quat_op(QOP_MEM_CMP, r1->data, r2->data));

	o_true = FALSE;
	switch (opcode) {
		case ZINST_BEQ: o_true = ( (int64_t)result == 0 ); break;
		case ZINST_BNE: o_true = ( (int64_t)result != 0 ); break;
		case ZINST_BLT: o_true = ( (int64_t)result < 0 ); break;
		case ZINST_BGE: o_true = ( (int64_t)result >= 0 ); break;
		case ZINST_BLTU: o_true = ( (uint64_t)result < 0 ); break;
		case ZINST_BGEU: o_true = ( (uint64_t)result > 0 ); break;
	}

	if (o_true) {
		/* jump to relative stack index. */
		z->pc += addr;
	}
	/* otherwise continue as normal.. */
}

void zpu_exec_alc(zpu_t *z, uint32_t opcode, zreg_t *rdest, zreg_t *r1, zreg_t *r2)
{
	const int qcode = QOP_MASK_MATH + (ZINST_OP(opcode) & 15);
	qvar result;
	int i;

fprintf(stderr, "DEBUG: zpu_exec_alc: qcode %d (%f, %f)\n", qcode, (double)quat_get(r1->data), (double)quat_get(r2->data));
	zpu_reg_set(z, rdest, quat_op(qcode, r1->data, r2->data));
}

zvar zpu_num(uint64_t val)
{
	num_t r_val = (num_t)val;
	zvar ret_var = (zvar)quat_alloc(Q_NUM, &r_val, sizeof(num_t)); 
fprintf(stderr, "DEBUG: ZPU_NUM: ret_var = %llu\n", (unsigned long long)quat_get(ret_var));
//	return ((zvar)quat_alloc(Q_NUM, &r_val, sizeof(num_t))); 
	return (ret_var);
}

int64_t zpu_push_arg(zpu_t *z, va_list argp)
{
	int64_t ret;

	ret = (int64_t)va_arg(argp, int64_t);
fprintf(stderr, "DEBUG: zpu_push_arg: %lld\n", (long long)ret);

	if (ret == ZREG_VAR) {
		qvar var = (qvar)va_arg(argp, qvar);
		int idx;

		/* store literal value in (>32) temporary register */
		zpu_reg_temp(z, quat_get(var), &idx); 
		ret = (int64_t)idx;
//		ret = (int64_t)quat_get(var);
//fprintf(stderr, "DEBUG: zpu_push_arg: ZREG_VAR: %lld\n", (long long)ret);
	}

	return (ret);
}

int zpu_push(zpu_t *z, uint32_t code, ...) //	~ int32_t rdest, int32_t src1, int32_t src2
{
	va_list argp;
	zinst_t *inst;
	qvar *ret_args;
	qvar arg;
	int64_t rd;
	int64_t imm;
	int64_t rs1;
	int64_t rs2;
	int arg_tot;
	int arg_max;
	int i_idx;
	int idx;

	/* operation to perform */
	i_idx = z->sp;
	if (z->sp == 0)
		z->sp = (MAX_ZPU_STACK - 1);
	else
		z->sp--;

	inst = &z->inst[i_idx];
	memset(inst, 0, sizeof(zinst_t)); /* DEBUG: NOTE: de-alloc prev */
	inst->type = ZINST_TYPE(code); 
	inst->op = ZINST_OP(code); 
	inst->subop = ZINST_SUBOP(code); 
fprintf(stderr, "DEBUG: PUSH[#%d]: %s (type:0x%x) (op:0x%x) (subop:0x%x) (opcode: %x)\n", i_idx, zpu_opcode_label(code), (int)inst->type, (int)inst->op, (int)inst->subop, code); 

	va_start(argp, code);
	switch (ZINST_FAMILY(code)) {
		case ZPU_FAMILY_R:
			rd = zpu_push_arg(z, argp);
			inst->rdest = (rd & 31);

			rs1 = zpu_push_arg(z, argp);
			inst->src1 = (rs1 & 31);

			rs2 = zpu_push_arg(z, argp);
			inst->src2 = (rs2 & 31);
			break;

		case ZPU_FAMILY_I:
			rd = zpu_push_arg(z, argp);
			inst->rdest = (rd & 31);

			rs1 = zpu_push_arg(z, argp);
			inst->src1 = (rs1 & 31);

			imm = zpu_push_arg(z, argp);
			{
				zinstI_t *I = (zinstI_t *)inst;
				I->addr = imm;
			}
			break;

		case ZPU_FAMILY_S:
		case ZPU_FAMILY_B:
			imm = zpu_push_arg(z, argp);
			inst->rdest = (imm & 31);

			rs1 = zpu_push_arg(z, argp);
			inst->src1 = (rs1 & 31);

			rs2 = zpu_push_arg(z, argp);
			inst->src2 = (rs2 & 31);
			break;

		case ZPU_FAMILY_U:
		case ZPU_FAMILY_J:
			rd = zpu_push_arg(z, argp);
			inst->rdest = (rd & 31);

			imm = zpu_push_arg(z, argp);
			{
				zinstU_t *U = (zinstU_t *)inst;
				U->addr = imm;
			}
			break;
	}
	va_end(argp);

	return (rd);
}

uint32_t zpu_opcode(zinst_t *inst)
{
	uint32_t code;

fprintf(stderr, "DEBUG: inst-type(%d) inst-op(%d) inst-subop(%d) opcode(%x)\n", (int)inst->type, (int)inst->op, (int)inst->subop, code);

	code = 
		(inst->type << ZINST_TYPE_SHIFT) + 
		(inst->op << ZINST_OP_SHIFT);
	
	if (ZINST_FAMILY(code) == ZPU_FAMILY_R)
		code += inst->subop;

	return (code);
}

void zpu_exec_inst(zpu_t *z, uint32_t opcode, zreg_t *rdest, zreg_t *r1, zreg_t *r2)
{
	off_t addr;
	int type;
	
	type = ZINST_TYPE(opcode);
fprintf(stderr, "DEBUG: zpu_exec_inst: opcode(0x%x) type(0x%x) {%x, %f, %f}\n", opcode, type, rdest, (double)quat_get(r1->data), quat_get(r2->data));
	switch (type) {
		case ZINST_TYPE_LOAD:
			addr = (off_t)quat_get(r1->data);
			zpu_exec_load(z, opcode, addr, rdest);
			break;
		case ZINST_TYPE_SAVE:
			addr = (off_t)quat_get(r1->data);
			zpu_exec_save(z, opcode, addr, rdest);
			break;
		case ZINST_TYPE_BRANCH:
			addr = (off_t)quat_get(rdest->data);
			zpu_exec_branch(z, opcode, addr, r1, r2);
			break;
		case ZINST_TYPE_ALCI:
		case ZINST_TYPE_ALC:
			zpu_exec_alc(z, opcode, rdest, r1, r2);
			break;
	}

}

void zpu_exec(zpu_t *z)
{
	zinst_t *inst;
	zreg_t *rd;
	zreg_t *r1;
	zreg_t *r2;
	uint32_t code;
	off_t addr;
	int fam;

	inst = zpu_pop(z);
fprintf(stderr, "DEBUG: EXEC: {%x} = zpu_pop (sp: %d) (pc: %d)\n", inst, z->sp, z->pc);
	if (!inst)
		return;

	rd = r1 = r2 = NULL;
	code = zpu_opcode(inst);
	fam = ZINST_FAMILY(code);
fprintf(stderr, "DEBUG: zpu_exec: opcode(0x%x) fam(0x%x) inst-type(0x%x) inst-op(0x%x)\n", (int)code,(int)fam, (int)inst->type, inst->op);
	switch (fam) {
		case ZPU_FAMILY_R:
			rd = zpu_reg_get(z, (int)inst->rdest);
			r1 = zpu_reg_get(z, (int)inst->src1);
			r2 = zpu_reg_get(z, (int)inst->src2);
fprintf(stderr, "DEBUG: EXEC: ZPU_FAMILY_R: inst-rdest(%d) inst-src1(%d) inst-src2(%d)\n", (int)inst->rdest, (int)inst->src1, (int)inst->src2);
			break;
		case ZPU_FAMILY_I:
			rd = zpu_reg_get(z, (int)inst->rdest);
			r1 = zpu_reg_get(z, (int)inst->src1);
			{
				zinstI_t *I = (zinstI_t *)inst;
				int64_t imm = (int64_t)I->addr;
				r2 = zpu_reg_temp(z, (num_t)imm, NULL);
			}
			break;
		case ZPU_FAMILY_B: /* branches */
			rd = zpu_reg_temp(z, (inst->rdest + (inst->subop << 5)), NULL);
			r1 = zpu_reg_get(z, (int)inst->src1);
			r2 = zpu_reg_get(z, (int)inst->src2);
			break;
		case ZPU_FAMILY_S: /* memory saving */
			addr = SIGN_EXTEND(inst->rdest + (inst->subop << 5)) + inst->rdest;
			rd = zpu_reg_temp(z, addr, NULL);
			r1 = zpu_reg_get(z, (int)inst->src1);
			r2 = zpu_reg_get(z, (int)inst->src2);
			break;
		case ZPU_FAMILY_J:
		case ZPU_FAMILY_U:
			rd = zpu_reg_get(z, (int)inst->rdest);
			{
				zinstU_t *U = (zinstU_t *)inst;
				int64_t imm = (int64_t)U->addr;
				r1 = zpu_reg_temp(z, (num_t)imm, NULL);
			}
			break;
	}

	zpu_exec_inst(z, code, rd, r1, r2);
}

zinst_t *zpu_peek(zpu_t *z, int stack_index)
{
	return (&z->inst[stack_index]);
}

zinst_t *zpu_pop(zpu_t *z)
{
	uint32_t idx = z->pc;
fprintf(stderr, "DEBUG: zpu_pop: #%d\n", idx);

	if (idx == 0)
		z->pc = (MAX_ZPU_STACK - 1);
	else
		z->pc--;

	return (&z->inst[idx]);
}

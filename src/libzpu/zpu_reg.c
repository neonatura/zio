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

#include "zpu.h"

zreg_t *zpu_reg_get(zpu_t *z, int addr)
{

	if (addr < 0 || addr >= z->reg_size)
		return (NULL);

	return ((zreg_t *)&z->reg[addr]);
}

void zpu_reg_set(zpu_t *z, zreg_t *reg, qvar value)
{
	uint8_t *ovalue;
	size_t nlen;
	size_t len;

fprintf(stderr, "DEBUG: zpu_reg_set: value %f\n", (double)quat_get(value));

	nlen = MAX(sizeof(qnum), quat_var_size(value));
	if (reg->size != nlen) {
		if (reg->data) free(reg->data);
		/* new value */
		reg->size = nlen;
		reg->data = (uint8_t *)calloc(reg->size, sizeof(uint8_t));
	} else {
		/* over-write */
		memset(reg->data, 0, reg->size);
	}
	memcpy(reg->data, value, nlen); 
}

void zpu_reg_nset(zpu_t *z, zreg_t *reg, num_t value)
{
	qnum num;

fprintf(stderr, "DEBUG: zpu_reg_nset: imm %f\n", (double)value);

	memset((uint8_t *)num, 0, sizeof(qnum));
	quat_set(value, Q_NUM, num);
	zpu_reg_set(z, reg, (qvar)num);
}

int zpu_reg_init(zpu_t *z, size_t max_size)
{

	if (z->reg)
		free(z->reg);

	z->reg = (zreg_t *)calloc(max_size, sizeof(zreg_t));
	if (!z->reg)
		return (-1);

	z->reg_size = max_size;
	return (0);
}

zreg_t *zpu_reg_temp(zpu_t *z, num_t value, int *idx_p)
{
	static int idx;
	zreg_t *reg;

	idx = (idx + 1) % 4;
	reg = (&z->reg[idx+28]);
fprintf(stderr, "DEBUG: zpu_temp_reg: temp reg #%d for value %f\n", idx, (double)value);
	zpu_reg_nset(z, reg, (num_t)value);

	if (idx_p)
		*idx_p = (idx + 28);

	return (reg);
}


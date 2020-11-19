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

#ifndef __ZPU_REG_H__
#define __ZPU_REG_H__

zreg_t *zpu_reg_get(zpu_t *z, int addr);

void zpu_reg_set(zpu_t *z, zreg_t *reg, qvar value);

void zpu_reg_nset(zpu_t *z, zreg_t *reg, num_t value);

int zpu_reg_init(zpu_t *z, size_t max_size);

zreg_t *zpu_reg_temp(zpu_t *z, num_t value, int *idx_p);

#endif /* ndef __ZPU_REG_H__ */

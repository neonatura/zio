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

#ifndef __ZPU_EXEC_H__
#define __ZPU_EXEC_H__

/** Add an instruction to the ZPU stack. */
int zpu_push(zpu_t *z, uint32_t code, ...);

/** Retrieve variable on the top of the stack and remove instruction from stack. */
zinst_t *zpu_pop(zpu_t *z);

qvar zpu_pop_var(zpu_t *z);

/** Retrieve a specific variable on the stack. */
zinst_t *zpu_pull(zpu_t *z, int stack_index);

zvar zpu_num(uint64_t val);

zinst_t *zpu_peek(zpu_t *z, int stack_index);

int zpu_exec(zprocessor_t *zproc, zpu_t *z);

#endif /* ndef __ZPU_EXEC_H__ */

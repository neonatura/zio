
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

int zpu_init(zpu_t *z)
{
	z->pc = z->sp = (MAX_ZPU_STACK-1);
	z->addr_logical_max = 0xFFFFFFFF;
	zpu_vaddr_init(z, MAX_ZPU_VADDR_SIZE);
	zpu_reg_init(z, MAX_ZPU_REGISTER_SIZE);
}

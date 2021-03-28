
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

zprocessor_t *zpu_init(int core_max, zaddr_t logical_max)
{
	zprocessor_t *zproc;
	zpu_t *z;
	int i;

	quat_init();

	zproc = (zprocessor_t *)calloc(1, sizeof(zprocessor_t));
	if (!zproc)
		return (NULL);

	zproc->page_table = (zdb_t **)calloc(logical_max / ZPU_VADDR_PAGE_SIZE, sizeof(zdb_t *));
	if (!zproc->page_table)
		return (NULL);

	zproc->core_max = core_max;
	zproc->logical_max = logical_max;

	zproc->core = (zpu_t *)calloc(core_max, sizeof(zpu_t));
	if (!zproc->core)
		return (NULL);

	zpu_vaddr_init(zproc);

	for (i = 0; i < core_max; i++) {
		z = &zproc->core[i];
		z->pc = z->sp = (MAX_ZPU_STACK-1);
	//	z->addr_logical_max = 0xEFFFFFFF;
	//	zpu_vaddr_init(z, z->addr_logical_max);
		zpu_reg_init(z, MAX_ZPU_REGISTER_SIZE);
	}

	return (zproc);
}

zpu_t *zpu_core(zprocessor_t *zproc, int core)
{

	if (core < 0 || core >= zproc->core_max)
		return (NULL);

	return (&zproc->core[core]);

}

void zpu_core_term(zpu_t *zpu)
{

	/* free registers */
	if (zpu->reg) {
		free(zpu->reg);
		zpu->reg = NULL;
		zpu->reg_size = 0;
	}

}


void zpu_free(zprocessor_t **zproc_p)
{
	zprocessor_t *zproc;
	int i;

	if (!zproc_p)
		return;

	zproc = *zproc_p;
	*zproc_p = NULL;
	if (!zproc)
		return;

	for (i = 0; i < zproc->core_max; i++) {
		zpu_core_term(&zproc->core[i]);
	}

	if (zproc->page_table) {
		zaddr_t page_max = zproc->logical_max / ZPU_VADDR_PAGE_SIZE; 
		for (i = 0; i < page_max; i++) {
			zdb_close(zproc->page_table[i]);
		}
		free(zproc->page_table);
	}

	free(zproc);
}


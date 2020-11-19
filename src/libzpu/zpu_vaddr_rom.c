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

uint8_t *zpu_vaddr_rom_get(zpu_t *z, size_t addr)
{
#if 0
	if (addr >= z->vaddr_size)
		return (NULL);

	if (!z->mem[addr].data)
		return (NULL);

	return (z->mem[addr].data);
#endif
	return (NULL);
}

int zpu_vaddr_rom_init(zpu_t *z, off_t page_addr)
{
#if 0

	if (z->mem)
		free(z->mem);

	z->mem = (zvaddr_t *)calloc(max_size, sizeof(zvaddr_t));
	if (z->mem)
		return (-1);

	z->vaddr_size = max_size;
	return (0);
#endif
}

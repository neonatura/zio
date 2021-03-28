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


uint8_t *zpu_vaddr_io_get(size_t addr)
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

int zpu_vaddr_io_init(off_t page_addr)
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

zdb_t *zpu_vaddr_io_table(int page_addr)
{
#if 0
	char buf[8];
	zdb_t *db;
	int err;

	if (page_addr < 0 || page_addr >= 0x200)
		return (NULL); /* IO: out of bounds */

	sprintf(buf, "io%-4.4x", (unsigned int)page_addr);
	err = zdb_open(buf, &db);
	if (err)
		return (NULL);

	dev_id = page_addr / 4;
	mode = (page_addr % 4);
	if (mode == 3)
		return (NULL);

	dev =  


	err = zpu_io_init(db, page_addr);
	if (err)
		return (err);

	err = zdb_get(db, 0, &data, &data_len);

	switch (mode) {
		case 0: /* bank 0 of lane fram */
			break;
		case 1: /* bank 1 of lane fram */
			break;
		case 2: /* i/o register */
			break;
	}


	return (db);
#endif
	return (NULL);
}

void zproc_io_init(zprocessor_t *zproc, zpu_ioconf_t *conf, zpu_iofifo_t *fifo)
{
	static int dev_id;
	int page;

	dev_id++;

	page = (dev_id * 4) + 2;
	zproc->page_table[page] = fifo->db;

}


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

/* DEBUG: */
#define zpu_vaddr_ram_table(_b) NULL
#define zpu_vaddr_io_table(_b) NULL

int zpu_vaddr_get(zprocessor_t *zproc, zaddr_t addr, uint8_t **data_p, size_t *data_len_p)
{
	zdb_t *db;
	uint8_t *ptr;
	zaddr_t page;
	int err;

#if 0
	if (addr >= z->addr_logical_max)
		return (ERR_INVAL); /* out of bounds */
#endif

	page = (addr / ZPU_VADDR_PAGE_SIZE);// (addr >> 12);
#if 0
	if (page > 0 && page < 0x10000)
		return (ERR_INVAL); /* reserved: kernel */
#endif

	*data_p = NULL;

fprintf(stderr, "DEBUG: page %d\n", page);

	/* retrieve data segment. */
	db = zproc->page_table[page];
	if (!db)
		return (ERR_OPNOTSUPP);

	err = zdb_get(db, (addr % ZPU_VADDR_PAGE_SIZE), data_p, data_len_p);
	if (err) {
		return (err);
	}

	return (0);
}

int zpu_vaddr_set(zprocessor_t *zproc, zaddr_t addr, uint8_t *data, size_t data_len)
{
	static zdb_hash_t blank_hash;
	zdb_t *db;
	zaddr_t page;
	int err;

#if 0
	if (addr >= z->addr_logical_max)
		return (ERR_INVAL); /* out of bounds */
#endif

	page = (addr / ZPU_VADDR_PAGE_SIZE);//(addr >> 12);
	if (page < 0x10000)
		return (ERR_OPNOTSUPP); /* read-only ROM */
#if 0
	if (page > 0x20000 && page < 0x30000)
		return (ERR_INVAL); /* reserved: kernel */
#endif

	/* store data segment. */
	db = zproc->page_table[page];
	if (!db)
		return (ERR_OPNOTSUPP);

	err = zdb_write(db, (addr % ZPU_VADDR_PAGE_SIZE), blank_hash, data, data_len);
	if (err)
		return (err);

	return (0);
}

int zpu_vaddr_init(zprocessor_t *zproc)
{
	const zaddr_t logical_max = zproc->logical_max;
	int max_page;
	int page;

	max_page = (logical_max / ZPU_VADDR_PAGE_SIZE);
	for (page = 0; page < max_page; page++) {
		if (page < 0x80) { /* kernel */
			zproc->page_table[page] = zpu_vaddr_ram_table(page);
		} else if (page < 0x100) { /* rom */
			zproc->page_table[page] = zpu_vaddr_rom_table(page - 0x80);
		} else if (page < 0x300) { /* i/o */
			zproc->page_table[page] = zpu_vaddr_io_table(page - 0x100);
		} else {
			zproc->page_table[page] = zpu_vaddr_ram_table(page);
		}
	}

}


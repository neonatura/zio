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
#define zpu_vaddr_rom_table(_a, _b) NULL
#define zpu_vaddr_ram_table(_a, _b) NULL
#define zpu_vaddr_io_table(_a, _b) NULL

int zpu_vaddr_get(zpu_t *z, zaddr_t addr, uint8_t **data_p, size_t *data_len_p)
{
	zdb_t *db;
	uint8_t *ptr;
	zaddr_t page;

	if (addr >= z->addr_logical_max)
		return (ERR_INVAL); /* out of bounds */

	page = (addr >> 12);
	if (page > 0 && page < 0x10000)
		return (ERR_INVAL); /* reserved: kernel */

	*data_p = NULL;

	/* retrieve data segment. */
	db = z->page_table[page];
	if (db)
		zdb_get(db, (addr & ZPU_VADDR_PAGE_SIZE), data_p, data_len_p);

	return (0);
}

int zpu_vaddr_set(zpu_t *z, zaddr_t addr, uint8_t *data, size_t data_len)
{
	static zdb_hash_t blank_hash;
	zdb_t *db;
	zaddr_t page;

	if (addr >= z->addr_logical_max)
		return (ERR_INVAL); /* out of bounds */

	page = (addr >> 12);
	if (page < 0x10000)
		return (ERR_OPNOTSUPP); /* read-only ROM */
	if (page > 0x20000 && page < 0x30000)
		return (ERR_INVAL); /* reserved: kernel */

	/* store data segment. */
	db = z->page_table[page];
	if (db)
		zdb_write(db, (addr % ZPU_VADDR_PAGE_SIZE), blank_hash, data, data_len);

	return (0);
}

int zpu_vaddr_init(zpu_t *z, size_t block_size)
{
	int max_page;
	int page;
	int fam;

	max_page = block_size / ZPU_VADDR_PAGE_SIZE; 
	for (page = 0; page < max_page; page++) {
		fam = page >> 14;
		if (fam < 0x1) {
			z->page_table[page] = zpu_vaddr_rom_table(z, page);
		} else if (fam < 0x2) {
			z->page_table[page] = zpu_vaddr_io_table(z, page - 0x10000);
		} else {
			z->page_table[page] = zpu_vaddr_ram_table(z, page - 0x20000);
		}
	}
}

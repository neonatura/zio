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


#define ZPU_ROM_CHARSET_VERSION 1

typedef struct romhdr_t {
	uint32_t version;
	uint32_t __unused0__;
} romhdr_t;

uint8_t *zpu_vaddr_rom_get(size_t addr)
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

int zpu_vaddr_rom_init(off_t page_addr)
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

int zpu_rom_version(zdb_t *db)
{
	romhdr_t hdr;
	uint8_t *data;
	size_t data_len;
	int err;

	memset(&hdr, 0, sizeof(hdr));
	err = zdb_get(db, 0, &data, &data_len);
	if (!err && data) {
		if (data_len >= sizeof(romhdr_t))
			memcpy(&hdr, data, sizeof(romhdr_t));
		free(data);
	}

	return ((int)hdr.version);
}

int zpu_rom_init(zdb_t *db, int version)
{
	romhdr_t hdr;
	uint8_t *raw = (uint8_t *)&hdr;
	size_t raw_len = sizeof(romhdr_t);

	memset(&hdr, 0, sizeof(hdr));
	hdr.version = (uint32_t)version;
	return (zdb_set(db, 0, raw, raw_len));
}

int zpu_rom_charset_init(zdb_t *db)
{
	image_font8_t f;
	uint8_t ch;
	int err;

	err = zpu_rom_init(db, ZPU_ROM_CHARSET_VERSION);
	if (err)
		return (err);

	for (ch = 1; ch < 255; ch++) {
		zfont_render(ch, &f);
		//image_t *f = zfont_init(ch, 0xFF, 1);
		qvar var = quat_alloc(Q_ARRAY, (uint8_t *)f.pixel, (f.width * f.height));
		zdb_set(db, (zdb_size_t)ch, var, quat_var_size(var));
		quat_free(&var);
//		zfont_free(&f);
	}

	return (0);
}

zdb_t *zpu_vaddr_rom_table(int page_addr)
{
	char buf[8];
	zdb_t *db;
	int err;

	if (page_addr < 0 || page_addr > 0x1FF)
		return (NULL); /* ROM: out of bounds */

	sprintf(buf, "rom%-4.4x", (unsigned int)page_addr);
	err = zdb_open(buf, &db);
	if (err)
		return (NULL);

	err = 0;
	switch (page_addr) {
		case ZADDR_ROM_PAGE_CHARSET:
			if (zpu_rom_version(db) != ZPU_ROM_CHARSET_VERSION) {
				err = zpu_rom_charset_init(db);
			}
			break;
	}

	return (db);
}


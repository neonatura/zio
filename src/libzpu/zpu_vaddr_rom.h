
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

#ifndef __ZPU_VADDR_ROM_H__
#define __ZPU_VADDR_ROM_H__

/*
 * ZPU ROM:
 *   0x0010 ASCII Font Charset
 */

/* The page offset of ROM memory. */
#define ZPU_VADDR_ROM_PAGE 0xF0000

/* The number of pages allocated to the ROM memory addressing. */
#define ZPU_VADDR_ROM_PAGE_SIZE 0xFFFF

#define ZADDR_ROM_PAGE_CHARSET 10

#define ZPU_ROM_ADDR(_page) \
	(ZPU_VADDR_ROM_PAGE * (_page))

/**
 * Address for rendered 6x8 8-bit white ASCII character.
 * @params _char An 8-bit ASCII character code from 1 to 254.
 */
#define ZPU_ROM_CHARSET_ADDR(_char) \
	(ZPU_ROM_ADDR(ZADDR_ROM_PAGE_CHARSET) + (_char % 256))

uint8_t *zpu_vaddr_rom_get(size_t addr);

int zpu_vaddr_rom_init(off_t page_addr);

zdb_t *zpu_vaddr_rom_table(int page_addr);

#endif /* ndef __ZPU_VADDR_H__ */


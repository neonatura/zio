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
 *   0xF000 ASCII Font Charset
 *   0xF010 
 */

/* The page offset of ROM memory. */
#define ZPU_VADDR_ROM_OFFSET 0xF0000
/* The number of pages allocated to the ROM memory addressing. */
#define MAX_ZPU_VADDR_ROM_SIZE 0xFFFF

uint8_t *zpu_vaddr_rom_get(zpu_t *z, size_t addr);

int zpu_vaddr_rom_init(zpu_t *z, off_t page_addr);

#endif /* ndef __ZPU_VADDR_H__ */

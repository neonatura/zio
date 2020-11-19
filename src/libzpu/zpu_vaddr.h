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

#ifndef __ZPU_VADDR_H__
#define __ZPU_VADDR_H__

/*
 * Logical address table: 
 *	0x00000 - 0x1FFFF ROM
 *	0x10000           IO:AUDIO
 *	0x10001           IO:COMM
 *	0x10002           IO:GEO
 *	0x10003           IO:THERM
 *	0x10004           IO:VIDEO
 *	0x20000 - 0x2FFFF RAM:KERNEL
 *	0x30000 - 0xFFFFF RAM:USER
*/

/* The size of each memory "chunk" or page. */
#define ZPU_VADDR_PAGE_SIZE 65536
/* The number of pages allocated to the memory addressing. */
#define MAX_ZPU_VADDR_SIZE 65536

int zpu_vaddr_init(zpu_t *z, size_t block_size);

int zpu_vaddr_set(zpu_t *z, size_t addr, uint8_t *data, size_t data_len);

int zpu_vaddr_get(zpu_t *z, zaddr_t addr, uint8_t **data_p, size_t *data_len_p);

#endif /* ndef __ZPU_VADDR_H__ */

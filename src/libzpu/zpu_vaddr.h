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
 * ZIO risc/v Logical address table (pages):
 *
 *   0x000 - 0x0FF  RAM:KERNEL
 *   0x080 - 0x0FF  ROM:KERNEL
 *   0x100 - 0x2FF  I/O
 *   0x300          RAM:USER
 *
 * Note: Each page is 65536 bytes of memory address space.
 */

/*
 * These onboard devices are always available at the following I/O page address:
 *   0x110 usb output uart stream (console uart output)
 *   0x150 system network adapter
 *   0x1A0 system time interface
 *   0x1D0 system usb hub
 *   0x1E0 bios video memory (vga/640x480/32bit)
 *   0x1F0 external keyboard / serial input (console uart input)
 */

/* The size of each memory "chunk" or page. */
#define ZPU_VADDR_PAGE_SIZE 65536

/* The number of pages allocated to the memory addressing. */
#define MAX_ZPU_VADDR_SIZE 65536

int zpu_vaddr_init(zprocessor_t *zproc);

int zpu_vaddr_set(zprocessor_t *zproc, zaddr_t addr, uint8_t *data, size_t data_len);

int zpu_vaddr_get(zprocessor_t *zproc, zaddr_t addr, uint8_t **data_p, size_t *data_len_p);

#endif /* ndef __ZPU_VADDR_H__ */


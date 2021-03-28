
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

#ifndef __ZPU_VADDR_IO_H__
#define __ZPU_VADDR_IO_H__

/*
 * ZPU IO:
 *   0x01 ASCII Font Charset
 */

/* The page offset of IO memory. */
#define ZPU_VADDR_IO_PAGE 0x100

/* The number of pages allocated to the IO memory addressing. */
#define ZPU_VADDR_IO_PAGE_SIZE ZPU_VADDR_PAGE_SIZE

#define ZPU_IO_ADDR(_page) \
	(ZPU_VADDR_IO_PAGE_SIZE * ((_page) + ZPU_VADDR_IO_PAGE))

uint8_t *zpu_vaddr_io_get(size_t addr);

int zpu_vaddr_io_init(off_t page_addr);

zdb_t *zpu_vaddr_io_table(int page_addr);

void zproc_io_init(zprocessor_t *zproc, zpu_ioconf_t *conf, zpu_iofifo_t *fifo);   

#endif /* ndef __ZPU_VADDR_H__ */


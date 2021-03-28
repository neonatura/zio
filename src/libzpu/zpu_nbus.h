
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

#ifndef __ZPU_NBUS_H__
#define __ZPU_NBUS_H__

/* neobus dma channel control */
#define NBUS_CTRL_ENABLE (1 << 0)
#define NBUS_CTRL_IDLE (1 << 1)
#define NBUS_CTRL_NOSRCINC (1 << 15)
#define NBUS_CTRL_NODSTINC (1 << 16)
#define NBUS_CTRL_16BIT (1 << 17)
#define NBUS_CTRL_32BIT (1 << 18)
#define NBUS_CTRL_INTEN (1 << 19)

#endif /* ndef __ZPU_NBUS_H__ */

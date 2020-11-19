/*
 * @copyright
 *
 *  Copyright 2020 Neo Natura
 *
 *  This file is part of the zio project.
 *  (https://github.com/neonatura/zio)
 *
 *  The zio project is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The zio project is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with zio project.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  @endcopyright
 */

#ifndef __CORE_H__
#define __CORE_H__

/* system */
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

/* libzsys */
#include "zsys.h"

/* libhtm */
#include "htm.h"

/* libzio */
#include "zio.h"

/* ziod */
#include "ziod_main.h"
#include "ziod_opt.h"
#include "ziod_therm.h"

#define RUN_NONE 0
#define RUN_IDLE 1

#endif /* ndef __CORE_H__ */

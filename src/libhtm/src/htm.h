
/*
 *  Copyright 2020 Brian Burrell
 *
 *  This file is part of libhtm.
 *  (https://github.com/neonatura/zio)
 *
 *  libhtm is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Shionbot is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with libhtm.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __HTM_H__
#define __HTM_H__

/* autoconf */
#include "config.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <limits.h>
#include <fcntl.h>
#ifdef HAVE_LIBM
#include <math.h>
#endif
#ifdef HAVE_LIBPTHREAD
#include <pthread.h>
extern int pthread_yield(void);
#endif

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE !FALSE
#endif

#ifndef MIN
#define MIN(a,b) \
	((a) < (b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b) \
	((a) > (b) ? (a) : (b))
#endif

#ifndef NAME_MAX
#define NAME_MAX 4095
#endif

#ifndef PATH_MAX
#define PATH_MAX NAME_MAX
#endif

/* "quat" module */
#include "quat/quat.h"

#include "htm_adler64.h"
#include "htm_struct.h"
#include "htm_entity.h"
#include "htm_brane.h"
#include "htm_cell.h"
#include "htm_path.h"
#include "htm_layer.h"
#include "htm_chord.h"
#include "htm_map.h"
#include "htm_mem.h"
#include "htm_exec.h"
#include "htm_exec_work.h"
#include "htm_emo.h"
#include "htm_neo.h"
#include "htm_tree.h"
#include "htm_cache.h"
#include "htm_control.h"
#include "htm_motor.h"
#include "htm_laplace.h"
#include "htm_celerity.h"
#include "htm_position.h"
#include "htm_zpu.h"
#include "util/htm_buff.h"
#include "util/htm_time.h"
#include "lang/lang_char.h"
#include "op/htm_op.h"

#endif /* __HTM_H__ */



/*
 *  Copyright 2020 Neo Natura
 *
 *  This file is part of the zio project.
 *  (https://github.com/neonatura/zio)
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

#ifndef __ZSYS_H__
#define __ZSYS_H__

#ifdef HAVE_CONFIG_H
/* autoconf */
#include "config.h"
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <inttypes.h>
#include <limits.h>
#include <fcntl.h>
#include <math.h>
#include <sys/select.h>
#include <arpa/inet.h>

#ifdef HAVE_LIBPTHREAD
#include <pthread.h>
extern int pthread_yield(void);
#endif

#ifndef PROGMEM
#define PROGMEM
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

#ifndef linux
#define getpagesize() sysconf(_SC_PAGESIZE)
#endif

typedef unsigned int uint;

#include "zsys_adler64.h"
#include "zsys_buff.h"
#include "zsys_error.h"
#include "zsys_image.h"
#include "zsys_font.h"
#include "zsys_laplace.h"
#include "zsys_log.h"
#include "zsys_map.h"
#include "zsys_path.h"
#include "zsys_string.h"
#include "zsys_time.h"

#endif /* ndef __ZSYS_H__ */


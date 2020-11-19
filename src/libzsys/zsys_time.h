/*
 * @copyright
 *
 *  Copyright 2018 Neo Natura
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

#ifndef __ZSYS_TIME_H__
#define __ZSYS_TIME_H__

#define ZTIME_NULL 0

#define ZEPOCH (ztime_t)946684800 

#define ztime_now() \
	ztime()

typedef uint64_t ztime_t;

void zmsleep(int msec);

void ztime_set(time_t ut);

void ztime_tmset(struct tm *tm);

time_t ztimeu(ztime_t ut);

ztime_t zutime(time_t ut);

struct tm *zgmtime(ztime_t stamp);

ztime_t ztimegm(struct tm *tm);

struct tm *zlocaltime(ztime_t stamp);

ztime_t ztimelocal(struct tm *tm);

ztime_t ztime(void);

#endif /* ndef __ZSYS_TIME_H__ */


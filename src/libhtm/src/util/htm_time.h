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

#ifndef __HTM_TIME_H__
#define __HTM_TIME_H__

#define HTM_TIME_NULL 0

#define HTM_EPOCH (htime_t)946684800 

#define htime_now() \
	htm_time()

typedef uint64_t htime_t;

void htm_msleep(int msec);

void htm_time_set(time_t ut);

void htm_time_tmset(struct tm *tm);

time_t htm_timeu(htime_t ut);

htime_t htm_utime(time_t ut);

struct tm *htm_gmtime(htime_t stamp);

htime_t htm_timegm(struct tm *tm);

struct tm *htm_localtime(htime_t stamp);

htime_t htm_timelocal(struct tm *tm);

htime_t htm_time(void);

#endif /* ndef __HTM_TIME_H__ */


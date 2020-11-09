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

#include "htm.h"

void htm_msleep(int msec)
{
	struct timeval tv;

	memset(&tv, 0, sizeof(tv));
	tv.tv_sec = msec / 1000;
	tv.tv_usec = (msec % 1000) * 1000;
	select(0, NULL, NULL, NULL, &tv);
}

void htm_time_set(time_t ut)
{
	struct timespec t;
	struct timeval tv;

	gettimeofday(&tv, NULL);
	if (tv.tv_sec == ut)
		return;

	/* adjust how many seconds we are off without changing usec */
	tv.tv_sec = (ut - tv.tv_sec);
	adjtime(&tv, NULL);
}

void htm_time_tmset(struct tm *tm)
{
	return (htm_time_set(mktime(tm)));
}

time_t htm_timeu(htime_t ut)
{
	return ( (ut / 1000) + HTM_EPOCH );
}

htime_t htm_utime(time_t ut)
{
	return ( ((ut - HTM_EPOCH) * 1000) );
}

struct tm *htm_gmtime(htime_t stamp)
{
	const time_t t = htm_timeu(stamp);
	return (gmtime(&t));	
}

htime_t htm_timegm(struct tm *tm)
{
	time_t t = timegm(tm);
	return (htm_utime(t));
}

struct tm *htm_localtime(htime_t stamp)
{
	const time_t t = htm_timeu(stamp);
	return (localtime(&t));	
}

htime_t htm_timelocal(struct tm *tm)
{
	time_t t = mktime(tm);
	return (htm_utime(t));
}

htime_t htm_time(void)
{
#ifdef HAVE_CLOCK_GETTIME
	struct timespec t;

	if (0 != clock_gettime(CLOCK_REALTIME, &t)) { /* -lrt */
		return (HTM_TIME_NULL);
	}

	if (t.tv_sec <= HTM_EPOCH)
		return (HTM_TIME_NULL);

	t.tv_sec -= HTM_EPOCH;
	return ( ((htime_t)t.tv_sec * 1000) + (htime_t)(t.tv_nsec/1000000) );
#else
	struct timeval tv;
	htime_t ret;

	memset(&tv, 0, sizeof(tv));
	gettimeofday(&tv, NULL);

	if (tv.tv_sec <= HTM_EPOCH)
		return (HTM_TIME_NULL);

	tv.tv_sec -= HTM_EPOCH;
	ret = ((htime_t)tv.tv_sec * 1000) + (htime_t)(tv.tv_usec / 1000);
	return (ret);
#endif
}


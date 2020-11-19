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

#include "zsys.h"

void zmsleep(int msec)
{
	struct timeval tv;

	memset(&tv, 0, sizeof(tv));
	tv.tv_sec = msec / 1000;
	tv.tv_usec = (msec % 1000) * 1000;
	select(0, NULL, NULL, NULL, &tv);
}

void ztime_set(time_t ut)
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

void ztime_tmset(struct tm *tm)
{
	return (ztime_set(mktime(tm)));
}

time_t ztimeu(ztime_t ut)
{
	return ( (ut / 1000) + ZEPOCH );
}

ztime_t zutime(time_t ut)
{
	return ( ((ut - ZEPOCH) * 1000) );
}

struct tm *zgmtime(ztime_t stamp)
{
	const time_t t = ztimeu(stamp);
	return (gmtime(&t));	
}

ztime_t ztimegm(struct tm *tm)
{
	time_t t = timegm(tm);
	return (zutime(t));
}

struct tm *zlocaltime(ztime_t stamp)
{
	const time_t t = ztimeu(stamp);
	return (localtime(&t));	
}

ztime_t ztimelocal(struct tm *tm)
{
	time_t t = mktime(tm);
	return (zutime(t));
}

ztime_t ztime(void)
{
#ifdef HAVE_CLOCK_GETTIME
	struct timespec t;

	if (0 != clock_gettime(CLOCK_REALTIME, &t)) { /* -lrt */
		return (ZTIME_NULL);
	}

	if (t.tv_sec <= ZEPOCH)
		return (ZTIME_NULL);

	t.tv_sec -= ZEPOCH;
	return ( ((ztime_t)t.tv_sec * 1000) + (ztime_t)(t.tv_nsec/1000000) );
#else
	struct timeval tv;
	ztime_t ret;

	memset(&tv, 0, sizeof(tv));
	gettimeofday(&tv, NULL);

	if (tv.tv_sec <= ZEPOCH)
		return (ZTIME_NULL);

	tv.tv_sec -= ZEPOCH;
	ret = ((ztime_t)tv.tv_sec * 1000) + (ztime_t)(tv.tv_usec / 1000);
	return (ret);
#endif
}


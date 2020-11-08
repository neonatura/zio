/* Copyright 2018 Neo Natura */

#include "zio.h"

#define ZIO_EPOCH (uint64_t)946684800 

void zio_msleep(int msec)
{
	struct timeval tv;

	memset(&tv, 0, sizeof(tv));
	tv.tv_sec = msec / 1000;
	tv.tv_usec = (msec % 1000) * 1000;
	select(0, NULL, NULL, NULL, &tv);
}

void zio_time_set(time_t ut)
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

void zio_time_tmset(struct tm *tm)
{
	return (zio_time_set(mktime(tm)));
}

time_t zio_timeu(uint64_t ut)
{
	return ( (ut / 1000) + ZIO_EPOCH );
}

uint64_t zio_utime(time_t ut)
{
	return ( ((ut - ZIO_EPOCH) * 1000) );
}

struct tm *zio_gmtime(uint64_t stamp)
{
	const time_t t = zio_timeu(stamp);
	return (gmtime(&t));	
}
uint64_t zio_timegm(struct tm *tm)
{
	time_t t = timegm(tm);
	return (zio_utime(t));
}
struct tm *zio_localtime(uint64_t stamp)
{
	const time_t t = zio_timeu(stamp);
	return (localtime(&t));	
}
uint64_t zio_timelocal(struct tm *tm)
{
	time_t t = mktime(tm);
	return (zio_utime(t));
}

uint64_t zio_time(void)
{
#ifdef HAVE_CLOCK_GETTIME
	struct timespec t;

	if (0 != clock_gettime(CLOCK_REALTIME, &t)) { /* -lrt */
		return (ZIO_TIME_NULL);
	}

	if (t.tv_sec <= ZIO_EPOCH)
		return (ZIO_TIME_NULL);

	t.tv_sec -= ZIO_EPOCH;
	return ( ((uint64_t)t.tv_sec * 1000) + (uint64_t)(t.tv_nsec/1000000) );
#else
	struct timeval tv;
	uint64_t ret;

	memset(&tv, 0, sizeof(tv));
	gettimeofday(&tv, NULL);

	if (tv.tv_sec <= ZIO_EPOCH)
		return (ZIO_TIME_NULL);

	tv.tv_sec -= ZIO_EPOCH;
	ret = ((uint64_t)tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (ret);
#endif
}


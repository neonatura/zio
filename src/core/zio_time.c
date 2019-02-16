/* Copyright 2018 Neo Natura */

#include "zio.h"

#define ZIO_EPOCH (uint64_t)946684800 

uint64_t zio_mtime(void)
{
#ifdef HAVE_CLOCK_GETTIME
	struct timespec t;

	if (0 != clock_gettime(CLOCK_MONOTONIC_RAW, &t)) { /* -lrt */
		return (ZIO_TIME_NULL);
	}

	t.tv_sec -= ZIO_EPOCH;
	return ( ((uint64_t)t.tv_sec * 1000) + (uint64_t)(t.tv_nsec/1000000) );
#else
	return ( (time(NULL) - ZIO_EPOCH) * 1000 );
#endif
}

time_t zio_time(void)
{
	return (time(NULL));
}


void zio_msleep(int msec)
{
	struct timeval tv;

	memset(&tv, 0, sizeof(tv));
	tv.tv_sec = msec / 1000;
	tv.tv_usec = msec % 1000;
	select(0, NULL, NULL, NULL, &tv);
}

void zio_time_tmset(struct tm *tm)
{
	struct timespec t;
	struct timeval tv;
	time_t ut;

	ut = mktime(tm);

	gettimeofday(&tv, NULL);
	if (tv.tv_sec == ut)
		return;

	/* adjust how many seconds we are off without changing usec */
	tv.tv_sec = (ut - tv.tv_sec);
	adjtime(&tv, NULL);
}


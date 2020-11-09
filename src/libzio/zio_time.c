/* Copyright 2018 Neo Natura */

#include "zio.h"
#include "util/htm_time.h"

struct tm *zio_gmtime(ztime_t stamp)
{
	return (htm_gmtime(stamp));
}

ztime_t zio_utime(time_t ut)
{
	return (htm_utime(ut));
}

time_t zio_timeu(ztime_t ut)
{
	return (htm_timeu(ut));
}

void zio_time_tmset(struct tm *tm)
{
	htm_time_tmset(tm);
}

void zio_msleep(int msec)
{
	htm_msleep(msec);
}

/* current system time */
ztime_t zio_time(void)
{
	return (htm_time());
}

void zio_time_set(time_t ut)
{
	htm_time_set(ut);
}

ztime_t zio_timegm(struct tm *tm)
{
	return (htm_timegm(tm));
}

struct tm *zio_localtime(ztime_t stamp)
{
	return (htm_localtime(stamp));
}

ztime_t zio_timelocal(struct tm *tm)
{
	return (htm_timelocal(tm));
}


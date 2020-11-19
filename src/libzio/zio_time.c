/* Copyright 2018 Neo Natura */

#include "zio.h"

struct tm *zio_gmtime(ztime_t stamp)
{
	return (zgmtime(stamp));
}

ztime_t zio_utime(time_t ut)
{
	return (zutime(ut));
}

time_t zio_timeu(ztime_t ut)
{
	return (ztimeu(ut));
}

void zio_time_tmset(struct tm *tm)
{
	ztime_tmset(tm);
}

void zio_msleep(int msec)
{
	zmsleep(msec);
}

/* current system time */
ztime_t zio_time(void)
{
	return (ztime());
}

void zio_time_set(time_t ut)
{
	ztime_set(ut);
}

ztime_t zio_timegm(struct tm *tm)
{
	return (ztimegm(tm));
}

struct tm *zio_localtime(ztime_t stamp)
{
	return (zlocaltime(stamp));
}

ztime_t zio_timelocal(struct tm *tm)
{
	return (ztimelocal(tm));
}


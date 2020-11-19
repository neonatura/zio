/* Copyright 2018 Neo Natura */

#ifndef __ZIO_TIME_H__
#define __ZIO_TIME_H__

#define ZIO_TIME_NULL ZTIME_NULL

typedef uint64_t ztime_t;

struct tm *zio_gmtime(ztime_t stamp);

ztime_t zio_utime(time_t ut);

time_t zio_timeu(ztime_t ut);

void zio_time_tmset(struct tm *tm);

void zio_msleep(int msec);

/* current system time */
ztime_t zio_time(void);

void zio_time_set(time_t ut);

ztime_t zio_timegm(struct tm *tm);

struct tm *zio_localtime(ztime_t stamp);

ztime_t zio_timelocal(struct tm *tm);

#endif /* __ZIO_TIME_H__ */


/* Copyright 2018 Neo Natura */

#ifndef __ZIO_TIME_H__
#define __ZIO_TIME_H__

#define ZIO_TIME_NULL 0

void zio_msleep(int msec);

struct tm *zio_gmtime(uint64_t stamp);

uint64_t zio_utime(time_t ut);

time_t zio_timeu(uint64_t ut);

void zio_time_tmset(struct tm *tm);

void zio_msleep(int msec);

/* current system time */
uint64_t zio_time(void);

void zio_time_set(time_t ut);

uint64_t zio_timegm(struct tm *tm);

struct tm *zio_localtime(uint64_t stamp);

uint64_t zio_timelocal(struct tm *tm);

#endif /* __ZIO_TIME_H__ */


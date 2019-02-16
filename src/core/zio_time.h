/* Copyright 2018 Neo Natura */

#ifndef __ZIO_TIME_H__
#define __ZIO_TIME_H__

#define ZIO_TIME_NULL 0

/* absolute (epoch) time in seconds. */
time_t zio_time(void);

/* relative (uptime) time in milliseconds. */
uint64_t zio_mtime(void);

void zio_msleep(int msec);

#endif /* __ZIO_TIME_H__ */

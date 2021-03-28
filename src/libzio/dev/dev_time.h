/* Copyright 2018 Neo Natura */

#ifndef __DEV_TIME_H__
#define __DEV_TIME_H__

extern zdev_t zio_time_device;

#define TIME_DEVICE() ((zdev_t *)(&zio_time_device))
#define REGISTER_TIME_DEVICE() (zio_dev_register(TIME_DEVICE()))

#endif /* __DEV_TIME_H__ */


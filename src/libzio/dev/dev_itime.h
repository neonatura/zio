/* Copyright 2018 Neo Natura */

#ifndef __DEV_ITIME_H__
#define __DEV_ITIME_H__

extern zdev_t zio_itime_device;

#define REGISTER_ITIME_DEVICE() (zio_dev_register(&zio_itime_device))

#endif /* __DEV_ITIME_H__ */


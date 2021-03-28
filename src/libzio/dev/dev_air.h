/* Copyright 2018 Neo Natura */

#ifndef __ZIO_AIR_H__
#define __ZIO_AIR_H__

extern zdev_t zio_air_device;

#define AIR_DEVICE() ((zdev_t *)(&zio_air_device))
#define REGISTER_AIR_DEVICE() (zio_dev_register(AIR_DEVICE()))

#endif /* ndef __ZIO_AIR_H__ */


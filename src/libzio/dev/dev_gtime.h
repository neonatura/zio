/* Copyright 2018 Neo Natura */

#ifndef __ZIO_GTIME_H__ 
#define __ZIO_GTIME_H__ 

extern zdev_t zio_gtime_device;

#define REGISTER_GTIME_DEVICE() (zio_dev_register(&zio_gtime_device))

#endif /* ndef __ZIO_GTIME_H__ */


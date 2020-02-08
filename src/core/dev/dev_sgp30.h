/* Copyright 2018 Neo Natura */

#ifndef __ZIO_SGP30_H__
#define __ZIO_SGP30_H__

extern zdev_t zio_sgp30_device;

#define REGISTER_SGP30_DEVICE() (zio_dev_register(&zio_sgp30_device))

#endif /* ndef __ZIO_SGP30_H__ */


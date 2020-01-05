/* Copyright 2018 Neo Natura */

#ifndef __ZIO_THERM_H__
#define __ZIO_THERM_H__

#define REGISTER_THERM_DEVICE() (zio_dev_register(&zio_therm_device))

extern zdev_t zio_therm_device;

double zio_therm_f(double c);

double zio_therm_k(double c);

#endif /* ndef __ZIO_THERM_H__ */


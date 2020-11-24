/* Copyright 2018 Neo Natura */

#ifndef __DEV_THERM_H__
#define __DEV_THERM_H__

#define THERM_DEVICE() ((zdev_t *)(&zio_therm_device))
#define REGISTER_THERM_DEVICE() (zio_dev_register(THERM_DEVICE()))

extern zdev_t zio_therm_device;

double zio_therm_f(double c);

double zio_therm_k(double c);

#endif /* ndef __DEV_THERM_H__ */


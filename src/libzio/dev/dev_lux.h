/* Copyright 2018 Neo Natura */

#ifndef __DEV_LUX_H__
#define __DEV_LUX_H__

#define LUX_DEVICE() ((zdev_t *)(&zio_lux_device))
#define REGISTER_LUX_DEVICE() (zio_dev_register(LUX_DEVICE()))

extern zdev_t zio_lux_device;

#endif /* ndef __DEV_LUX_H__ */


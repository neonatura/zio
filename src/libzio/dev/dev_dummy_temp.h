/* Copyright 2018 Neo Natura */

#ifndef __DEV_DUMMY_TEMP_H__
#define __DEV_DUMMY_TEMP_H__

extern zdev_t zio_dummy_temp_device;

#define REGISTER_DUMMY_TEMP_DEVICE() (zio_dev_register(&zio_dummy_temp_device))

#endif /* ndef __DEV_DUMMY_TEMP_H__ */


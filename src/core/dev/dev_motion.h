/* Copyright 2019 Neo Natura */

#ifndef __DEV_MOTION_H__
#define __DEV_MOTION_H__

extern zdev_t zio_motion_device;

#define REGISTER_MOTION_DEVICE() (zio_dev_register(&zio_motion_device))

#endif /* ndef __DEV_MOTION_H__ */


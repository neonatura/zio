/* Copyright 2018 Neo Natura */

#ifndef __DEV_DEBUG_H__
#define __DEV_DEBUG_H__

extern zdev_t zio_debug_device;

#define REGISTER_DEBUG_DEVICE() (zio_dev_register(&zio_debug_device))

#endif /* ndef __DEV_DEBUG_H__ */


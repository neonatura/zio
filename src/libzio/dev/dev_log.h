/* Copyright 2018 Neo Natura */

#ifndef __DEV_LOG_H__
#define __DEV_LOG_H__

extern zdev_t zio_log_device;

#define LOG_DEVICE() ((zdev_t *)(&zio_log_device))
#define REGISTER_LOG_DEVICE() (zio_dev_register(LOG_DEVICE()))

#endif /* ndef __DEV_LOG_H__ */


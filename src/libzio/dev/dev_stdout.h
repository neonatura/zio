/* Copyright 2020 Neo Natura */

#ifndef __DEV_STDOUT_H__
#define __DEV_STDOUT_H__

extern zdev_t zio_stdout_device;

#define REGISTER_STDOUT_DEVICE() (zio_dev_register(&zio_stdout_device))

#define REGISTER_STATUS_DEVICE REGISTER_STDOUT_DEVICE

#endif /* ndef __DEV_STDOUT_H__ */

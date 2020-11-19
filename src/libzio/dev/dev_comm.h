/* Copyright 2020 Neo Natura */

#ifndef __DEV_COMM_H__
#define __DEV_COMM_H__

extern zdev_t zio_comm_device;

#define REGISTER_COMM_DEVICE() (zio_dev_register(&zio_comm_device))

#endif /* ndef __DEV_COMM_H__ */

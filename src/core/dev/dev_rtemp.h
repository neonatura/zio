/* Copyright 2018 Neo Natura */

#ifndef __ZIO_RTEMP_H__
#define __ZIO_RTEMP_H__

extern zdev_t zio_rtemp_device;

#define REGISTER_RTEMP_DEVICE() (zio_dev_register(&zio_rtemp_device))

#endif /* ndef __ZIO_RTEMP_H__ */


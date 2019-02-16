/* Copyright 2018 Neo Natura */

#ifndef __ZIO_DEBUG_H__
#define __ZIO_DEBUG_H__

#define SYS_DEBUG_PATH "/var/log/zio.log"
 
extern zdev_t zio_debug_device;

#define REGISTER_DEBUG_DEVICE() (zio_dev_register(&zio_debug_device))

#endif /* ndef __ZIO_DEBUG_H__ */


/* Copyright 2018 Neo Natura */

#ifndef __DEV_DIAG_H__
#define __DEV_DIAG_H__

#define SYS_DIAG_PATH "/var/log/zio.log"
 
extern zdev_t zio_diag_device;

#define REGISTER_DIAG_DEVICE() (zio_dev_register(&zio_diag_device))

#endif /* ndef __DEV_DIAG_H__ */


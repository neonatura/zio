/* Copyright 2018 Neo Natura */

#ifndef __DEV_ITEMP_H__
#define __DEV_ITEMP_H__

#define SYS_ITEMP_PATH "/sys/class/thermal/thermal_zone0/temp"
 
extern zdev_t zio_itemp_device;

#define REGISTER_ITEMP_DEVICE() (zio_dev_register(&zio_itemp_device))

#endif /* ndef __DEV_ITEMP_H__ */


/* Copyright 2018 Neo Natura */

#ifndef __DEV_LDR_H__
#define __DEV_LDR_H__

extern zdev_t zio_ldr0_device;
extern zdev_t zio_ldr1_device;

#define REGISTER_LDR0_DEVICE() (zio_dev_register(&zio_ldr0_device))
#define REGISTER_LDR1_DEVICE() (zio_dev_register(&zio_ldr1_device))

#define REGISTER_LDR_DEVICE REGISTER_LDR0_DEVICE

#endif /* ndef __DEV_LDR_H__ */


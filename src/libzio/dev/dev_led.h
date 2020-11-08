/* Copyright 2018 Neo Natura */

#ifndef __DEV_LED_H__
#define __DEV_LED_H__

extern zdev_t zio_led0_device;
extern zdev_t zio_led1_device;

#define REGISTER_LED0_DEVICE() (zio_dev_register(&zio_led0_device))
#define REGISTER_LED1_DEVICE() (zio_dev_register(&zio_led1_device))

#define REGISTER_LED_DEVICE REGISTER_LED0_DEVICE

#endif /* ndef __DEV_LED_H__ */


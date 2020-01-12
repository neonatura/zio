/* Copyright 2018 Neo Natura */

#ifndef __DEV_THERMISTOR_H__
#define __DEV_THERMISTOR_H__

extern zdev_t zio_thermistor0_device;
extern zdev_t zio_thermistor1_device;
extern zdev_t zio_thermistor2_device;
extern zdev_t zio_thermistor3_device;

#define REGISTER_THERMISTOR0_DEVICE() (zio_dev_register(&zio_thermistor0_device))
#define REGISTER_THERMISTOR1_DEVICE() (zio_dev_register(&zio_thermistor1_device))
#define REGISTER_THERMISTOR2_DEVICE() (zio_dev_register(&zio_thermistor2_device))
#define REGISTER_THERMISTOR3_DEVICE() (zio_dev_register(&zio_thermistor3_device))

#define REGISTER_THERMISTOR_DEVICE REGISTER_THERMISTOR0_DEVICE

#endif /* ndef __DEV_THERMISTOR_H__ */


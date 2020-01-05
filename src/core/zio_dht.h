/* Copyright 2018 Neo Natura */

#ifndef __ZIO_DHT_H__
#define __ZIO_DHT_H__

extern zdev_t zio_dht0_device;
extern zdev_t zio_dht1_device;
extern zdev_t zio_dht2_device;

#define REGISTER_DHT0_DEVICE() (zio_dev_register(&zio_dht0_device))
#define REGISTER_DHT1_DEVICE() (zio_dev_register(&zio_dht1_device))
#define REGISTER_DHT2_DEVICE() (zio_dev_register(&zio_dht2_device))

#define REGISTER_DHT_DEVICE REGISTER_DHT0_DEVICE

#endif /* ndef __ZIO_DHT_H__ */


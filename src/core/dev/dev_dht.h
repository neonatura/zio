/* Copyright 2018 Neo Natura */

#ifndef __DEV_DHT_H__
#define __DEV_DHT_H__

extern zdev_t zio_dht0_device;
extern zdev_t zio_dht1_device;
extern zdev_t zio_dht2_device;

#define REGISTER_DHT0_DEVICE() (zio_dev_register(&zio_dht0_device))
#define REGISTER_DHT1_DEVICE() (zio_dev_register(&zio_dht1_device))
#define REGISTER_DHT2_DEVICE() (zio_dev_register(&zio_dht2_device))

#define REGISTER_DHT_DEVICE REGISTER_DHT0_DEVICE

double zio_dht_humidity(void);

#endif /* ndef __DEV_DHT_H__ */


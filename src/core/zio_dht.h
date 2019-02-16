/* Copyright 2018 Neo Natura */

#ifndef __ZIO_DHT_H__
#define __ZIO_DHT_H__

#define MAX_DHT_TIMINGS	85

extern zdev_t zio_dht_device;

#define REGISTER_DHT_DEVICE() (zio_dev_register(&zio_dht_device))

#endif /* ndef __ZIO_DHT_H__ */


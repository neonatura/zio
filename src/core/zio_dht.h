/* Copyright 2018 Neo Natura */

#ifndef __ZIO_DHT_H__
#define __ZIO_DHT_H__

#include "zio.h"
 
#define MAX_DHT_TIMINGS	85

#define ZIO_DHT_MIN_MS 2000
#define ZIO_DHT_MAX_MS 16000
#define ZIO_DHT_MW 1
 
void zio_dht_init(zdev_t *dev);
 
void zio_dht_read(zdev_t *dev);

void zio_dht_term(zdev_t *dev);

int zdev_temp_c(zdev_t *dev);

int zdev_temp_f(zdev_t *dev);

int zio_dev_humidity(zdev_t *dev);


#endif /* ndef __ZIO_DHT_H__ */


/* Copyright 2018 Neo Natura */

#ifndef __ZIO_GPS_H__ 
#define __ZIO_GPS_H__ 


#define ZIO_GPS_PWR PIN_NULL
#define ZIO_GPS_MIN_MS 100
#define ZIO_GPS_MAX_MS 1000
#define ZIO_GPS_MW 100


int zio_gps_init(zdev_t *dev);
int zio_gps_read(zdev_t *dev);


#endif /* ndef __ZIO_GPS_H__ */


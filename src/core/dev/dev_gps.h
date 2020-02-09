/* Copyright 2018 Neo Natura */

#ifndef __ZIO_GPS_H__ 
#define __ZIO_GPS_H__ 

extern zdev_t zio_gps_device;

#define REGISTER_GPS_DEVICE() (zio_dev_register(&zio_gps_device))

double zio_geo_latitude(void);

double zio_geo_longitude(void);

zgeo_t *zio_geo_parse_GGA(zdev_t *dev, char *text);

zgeo_t *zio_geo_parse_GLL(zdev_t *dev, char *text);

zgeo_t *zio_geo_parse_RMC(zdev_t *dev, char *text);

int zio_geo_time_set(zdev_t *dev, char *time_str, char *day_str);

void zio_geo_latitude_set(double lat);

void zio_geo_longitude_set(double lon);

void zio_geo_set(zgeo_t *geo, double lat, double lon);

#endif /* ndef __ZIO_GPS_H__ */


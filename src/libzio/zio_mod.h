/* Copyright 2018 Neo Natura */

#ifndef __ZIO_MOD_H__
#define __ZIO_MOD_H__

double zio_mod_dvalue_avg(zdev_t *mod);

uint64_t zio_mod_time_avg(zdev_t *mod);

void zio_mod_write(zdev_t *mod, uint8_t *data, size_t data_len);

zgeo_t *zio_mod_geo_avg(zdev_t *mod);

#endif /* ndef __ZIO_MOD_H__ */


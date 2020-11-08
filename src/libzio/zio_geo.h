
/* Copyright 2018 Neo Natura */

#ifndef __ZIO_GEO_H__
#define __ZIO_GEO_H__

zgeo_t *zio_geo(void);

double zio_geo_dist(double lat1, double lon1, double lat2, double lon2);

zgrid_t *zio_grid_init(double lat, double lon);

int zio_grid_plot(zgrid_t *grid, zobj_t *obj, double lat, double lon);

int zio_grid_draw(zgrid_t *grid, uint8_t *buf, size_t buf_max);

int zio_grid_count(zgrid_t *grid);

int zio_grid_obj(zgrid_t *grid, int x, int y);

void zio_grid_obj_set(zgrid_t *grid, zobj_t *obj, int x, int y);

#endif /* ndef __ZIO_GEO_H__ */


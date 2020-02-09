/* Copyright 2018 Neo Natura */

#include "zio.h"

double zio_mod_dvalue_avg(zdev_t *mod)
{
	zdev_t *dev;
	double ret;
	double d;
	uint32_t ret_tot;
	uint32_t fact;

	ret = 0;
	ret_tot = 0;
	for (dev = zio_device_table; dev; dev = dev->next) {
		if (mod->module != dev->module)
			continue;
		if (mod->type != dev->type)
			continue;
		if (dev->flags & DEVF_MODULE)
			continue;

		d = zio_dvalue_avg(dev, MAX_VALUE_DOUBLE_SIZE-1);
		if (d == 0.00)
			continue;

		fact = MAX(0, 255 - dev->stratum);
		ret += (double)d * (double)fact;
		ret_tot += fact;
	}
	if (ret_tot != 0)
		ret /= ret_tot;

	return (ret);
}

uint64_t zio_mod_time_avg(zdev_t *mod)
{
	zdev_t *dev;
	uint64_t ret;
	uint64_t val;
	uint64_t off;
	uint32_t ret_tot;
	uint32_t fact;

	ret = 0;
	ret_tot = 0;
	for (dev = zio_device_table; dev; dev = dev->next) {
		if (mod->module != dev->module)
			continue;
		if (mod->type != dev->type)
			continue;
		if (dev->flags & DEVF_MODULE)
			continue;

		off = zio_fifo_span(dev);
		val = zio_value_get(dev) + off;

		fact = MAX(0, 255 - dev->stratum);
		ret += val * fact;
		ret_tot += fact;
	}
	if (ret_tot != 0)
		ret /= ret_tot;

	return (ret);
}

void zio_mod_write(zdev_t *mod, uint8_t *data, size_t data_len)
{
	zdev_t *dev;
	double ret;
	double d;
	uint32_t ret_tot;
	uint32_t fact;

	ret = 0;
	ret_tot = 0;
	for (dev = zio_device_table; dev; dev = dev->next) {
		if (mod->module != dev->module)
			continue;
		if (mod->type != dev->type)
			continue;
		if (dev->flags & DEVF_MODULE)
			continue;

		zio_write(dev, data, data_len);
	}

}

/* todo: stratum */
zgeo_t *zio_mod_geo_avg(zdev_t *mod)
{
	static zgeo_t ret_geo;
	zdev_t *g_dev;
	zgeo_t *geo;
	double lat, lon;
	double alt;
	int tot;

	tot = 0;
	alt = 0;
	lat = lon = 0;
	for (g_dev = zio_device_table; g_dev; g_dev = g_dev->next) {
		if (g_dev->flags & DEVF_MODULE)
			continue;
		if (g_dev->type != mod->type) // ZDEV_GEO
			continue;

		geo = zio_geo_value(g_dev);
		if (!geo)
			continue;

		lat += geo->lat;
		lon += geo->lon;
		alt += geo->alt;
		tot++;
	}
	if (tot == 0)
		return (NULL);

	memset(&ret_geo, 0, sizeof(ret_geo));
	ret_geo.lat = lat / (double)tot;
	ret_geo.lon = lon / (double)tot;
	ret_geo.alt = alt / (double)tot;
	return (&ret_geo);
}



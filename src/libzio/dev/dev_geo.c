/* Copyright 2018 Neo Natura */

#include "zio.h"

int zio_geo_open(zdev_t *dev)
{


	zio_dev_on(dev);
	return (0);
}

int zio_geo_read(zdev_t *dev)
{
	zgeo_t *geo;

	geo = zio_mod_geo_avg(dev);
	if (geo) {
		/* retain recent location as device's data payload. */
		zio_data_append(dev, (uint8_t *)geo, sizeof(zgeo_t));

		if (!dev->grid) {
			/* initialize a grid to track the area. */
			dev->grid = zio_grid_init(geo->lat, geo->lon);
		}
		if (dev->grid) {
			/* plot self on grid */
			zio_grid_plot(dev->grid, zio_obj_self(), geo->lat, geo->lon);
		}
	}

	return (0);
}

int zio_geo_poll(zdev_t *dev)
{
	int err;

	if (!is_zio_dev_on(dev))
		return (ERR_INVAL);

	err = zio_geo_read(dev);
	if (err)
		return (err);

	return (0);
}

int zio_geo_close(zdev_t *dev)
{
	zio_dev_off(dev);
	return (0);
}

int zio_geo_print(zdev_t *dev, int mode, void *retbuf)
{
	zgeo_t *geo;
	char buf[256];

	geo = zio_geo_value(dev);
	if (!geo)
		return (ERR_AGAIN);

	sprintf(buf, "@%f,%f", geo->lat, geo->lon);
	strcpy((char *)retbuf, buf);

	return (0);
}

zdev_t zio_geo_device =
{
	"geo", PIN_NULL, 0, /* geodetic location module */
	ZDEV_GEO, DEVF_START | DEVF_MODULE, ZMOD_INTERNAL,
	/* op */
	{ zio_geo_open, zio_geo_read, NULL, zio_geo_print, zio_geo_close, zio_geo_poll },
	/* param */
	{ /* freq_min */ 20, /* freq_max */ 40, 0, PIN_NULL }
};


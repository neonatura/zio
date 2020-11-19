/* Copyright 2018 Neo Natura */

#include "zio.h"

int zio_air_open(zdev_t *dev)
{

	zio_dev_on(dev);
	return (0);
}

int zio_air_read(zdev_t *dev)
{
	double d;

	d = zio_mod_dvalue_avg(dev);
	if (d == 0.00)
		return (ERR_AGAIN); /* skip */

	/* store last obtained value. */
	zio_dvalue_set(dev, d);

	return (0);
}

int zio_air_poll(zdev_t *dev)
{
        int err;

        if (is_zio_dev_on(dev)) {
                err = zio_air_read(dev);
                if (err)
                        return (err);
        }

        return (0);
}

int zio_air_close(zdev_t *dev)
{
	zio_dev_off(dev);
	return (0);
}

int zio_air_print(zdev_t *dev, int mode, void *retbuf)
{
	sprintf(retbuf, "%f%%", zio_dvalue_avg(dev, 10));
	return (0);
}

zdev_t zio_air_device =
{
	"air", PIN_NULL, 0, /* air quality module */
	ZDEV_AIR, DEVF_START | DEVF_MODULE, ZMOD_INTERNAL,
	/* op */
	{ zio_air_open, zio_air_read, NULL, zio_air_print, zio_air_close, zio_air_poll },
	/* param */
	{ /* freq_min */ 60, /* freq_max */ 120, 0, PIN_NULL }
};


/* Copyright 2018 Neo Natura */

#include "zio.h"
 
int zio_lux_open(zdev_t *dev)
{

	zio_dev_on(dev);

	return (0);
}
 
int zio_lux_read(zdev_t *dev)
{
	double d;

	d = zio_mod_dvalue_avg(dev);
	if (d == 0.00)
		return (ERR_AGAIN); /* skip */

	/* store last obtained value. */
	zio_dvalue_set(dev, d);

	return (0);
}

int zio_lux_poll(zdev_t *dev)
{
	int err;
	if (is_zio_dev_on(dev)) {
		err = zio_lux_read(dev);
		if (err)
			return (err);
	}
	return (0);
}

int zio_lux_close(zdev_t *dev)
{
	zio_dev_off(dev);
	return (0);
}

int zio_lux_print(zdev_t *dev, int mode, void *retbuf)
{
	char buf[256];

	sprintf(retbuf, "%-2.2fl", zio_dvalue_avg(dev, MAX_VALUE_DOUBLE_SIZE));

	return (0);
}

zdev_t zio_lux_device =
{
	"lux", PIN_NULL, 0, /* contoller: internal temperature */
	ZDEV_PHOTO, DEVF_START | DEVF_MODULE, ZMOD_INTERNAL, 
	/* op */
	{ zio_lux_open, zio_lux_read, NULL, zio_lux_print, zio_lux_close, zio_lux_poll },
	/* param */
	{ /* freq_min */ 60, /* freq_max */ 120, 0, PIN_NULL }
};



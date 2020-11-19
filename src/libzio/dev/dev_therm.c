/* Copyright 2018 Neo Natura */

#include "zio.h"
 
int zio_therm_open(zdev_t *dev)
{

	zio_dev_on(dev);

	return (0);
}
 
int zio_therm_read(zdev_t *dev)
{
	double d;

	d = zio_mod_dvalue_avg(dev);
	if (d == 0.00)
		return (ERR_AGAIN); /* skip */

	/* store last obtained value. */
	zio_dvalue_set(dev, d);

	return (0);
}

int zio_therm_poll(zdev_t *dev)
{
	int err;
	if (is_zio_dev_on(dev)) {
		err = zio_therm_read(dev);
		if (err)
			return (err);
	}
	return (0);
}

int zio_therm_close(zdev_t *dev)
{
	zio_dev_off(dev);
	return (0);
}

int zio_therm_print(zdev_t *dev, int mode, void *retbuf)
{
	char buf[256];

	if (mode == ZIO_FMT_FAHRENHEIT) {
		sprintf(retbuf, "%-2.2fF", zio_therm_f(zio_dvalue_avg(dev, 4)));
	} else if (mode == ZIO_FMT_KELVIN) {
		sprintf(retbuf, "%-2.2fK", zio_therm_k(zio_dvalue_avg(dev, 4)));
	} else /* mode == ZIO_FMT_CELSIUS */ {
		sprintf(retbuf, "%-2.2fC", zio_dvalue_avg(dev, 4));
	}

	return (0);
}

double zio_therm_k(double c)
{
	return (c + 273.15);
}

double zio_therm_f(double c)
{
	return ((c * 1.8) + 32);
}

zdev_t zio_therm_device =
{
	"therm", PIN_NULL, 0, /* contoller: internal temperature */
	ZDEV_THERM, DEVF_START | DEVF_MODULE, ZMOD_INTERNAL, 
	/* op */
	{ zio_therm_open, zio_therm_read, NULL, zio_therm_print, zio_therm_close, zio_therm_poll },
	/* param */
	{ /* freq_min */ 60, /* freq_max */ 120, 0, PIN_NULL }
};



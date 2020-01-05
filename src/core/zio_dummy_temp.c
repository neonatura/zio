/* Copyright 2018 Neo Natura */

#include "zio.h"
 
#define NOMINAL_TEMP 25
 
int zio_dummy_temp_open(zdev_t *dev)
{

	zio_dev_on(dev);

	return (0);
}
 
int zio_dummy_temp_read(zdev_t *dev)
{

	/* store dummy temperature value. */
	zio_dvalue_set(dev, NOMINAL_TEMP);

	return (0);
}

int zio_dummy_temp_poll(zdev_t *dev)
{
	int err;
	if (is_zio_dev_on(dev)) {
		err = zio_dummy_temp_read(dev);
		if (err)
			return (err);
	}
	return (0);
}

int zio_dummy_temp_close(zdev_t *dev)
{
	zio_dev_off(dev);
	return (0);
}

int zio_dummy_temp_print(zdev_t *dev, int mode, void *retbuf)
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

zdev_t zio_dummy_temp_device =
{
	"dummy_temp", PIN_NULL, STRATUM_MAX, /* contoller: internal temperature */
	ZDEV_THERM, DEVF_START | DEVF_INPUT | DEVF_DUMMY, ZMOD_INTERNAL, 
	/* op */
	{ zio_dummy_temp_open, zio_dummy_temp_read, NULL, zio_dummy_temp_print, zio_dummy_temp_close, zio_dummy_temp_poll },
	/* param */
	{ /* freq_min */ 1, /* freq_max */ 10 }
};


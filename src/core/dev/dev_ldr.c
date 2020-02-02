/* Copyright 2018 Neo Natura */
 
#include "zio.h"

int zio_ldr_open(zdev_t *dev)
{
	zio_dev_on(dev);
	return (0);
}

static double _calc_lux(double voltage)
{
	return (27.565 * exp10(3.3 - voltage));
}

int zio_ldr_read(zdev_t *dev)
{
	double volt;
	double temp;
	double val;
	double r;

	val = (double)analogRead(dev->def_pin);
	if (val == 0.0 || val == 255.0)
		return (ZERR_AGAIN);

	volt = 3.3 / 255 * val;

	/* store last obtained value. */
	zio_dvalue_set(dev, _calc_lux(volt));

	return (0);
}

int zio_ldr_poll(zdev_t *dev)
{
	int err;

	if (is_zio_dev_on(dev)) {
		err = zio_ldr_read(dev);
		if (err)
			return (err);
	}

	return (0);
}

int zio_ldr_print(zdev_t *dev, int mode, void *retbuf)
{
	char buf[256];

	sprintf(retbuf, "%-2.2fl", zio_dvalue_avg(dev, 4));

	return (0);
}

int zio_ldr_close(zdev_t *dev)
{
	zio_dev_off(dev);
}

zdev_t zio_ldr0_device =
{
	"ldr0", PIN_A0, 1, /* contoller: analog temperature */
	ZDEV_PHOTO, DEVF_START | DEVF_INPUT, ZMOD_INTERNAL,
	/* op */
	{ zio_ldr_open, zio_ldr_read, NULL, zio_ldr_print, zio_ldr_close, zio_ldr_poll },
	/* param */
	{ /* freq_min */ 1, /* freq_max */ 10, 0.002, PIN_NULL }
};

zdev_t zio_ldr1_device =
{
	"ldr1", PIN_A2, 1, /* contoller: analog temperature */
	ZDEV_PHOTO, DEVF_START | DEVF_INPUT, ZMOD_INTERNAL,
	/* op */
	{ zio_ldr_open, zio_ldr_read, NULL, zio_ldr_print, zio_ldr_close, zio_ldr_poll },
	/* param */
	{ /* freq_min */ 1, /* freq_max */ 10, 0.002, PIN_NULL }
};


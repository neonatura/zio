/* Copyright 2018 Neo Natura */

#include "zio.h"

int zio_motion_open(zdev_t *dev)
{
	int err;

	if (dev->def_pin == PIN_NULL)
		return (ERR_INVAL);

	err = zio_dev_on(dev);
	if (err)
		return (err);

	PIN_MODE(dev->def_pin, INPUT);

	return (0);
}
 
int zio_motion_read(zdev_t *dev)
{
	int val = 0;

	if (DIGITAL_READ(dev->def_pin))
		val = 1;

	zio_ivalue_set(dev, val);

	return (0);
}

int zio_motion_poll(zdev_t *dev)
{
	int err;
	if (is_zio_dev_on(dev)) {
		err = zio_motion_read(dev);
		if (err)
			return (err);
	}
	return (0);
}

int zio_motion_close(zdev_t *dev)
{
	zio_dev_off(dev);
	return (0);
}

int zio_motion_print(zdev_t *dev, int mode, void *retbuf)
{
	char buf[256];

	if (zio_ivalue_get(dev)) {
		strcpy(retbuf, "true");
	} else {
		strcpy(retbuf, "false");
	}

	return (0);
}

zdev_t zio_motion0_device =
{
	"motion0", PIN_MOTION_0, 1,
	ZDEV_MOTION, DEVF_START | DEVF_INPUT, ZMOD_EXTERNAL,
	/* op */
	{ zio_motion_open, zio_motion_read, NULL, zio_motion_print, zio_motion_close, zio_motion_poll },
	/* param */
	{ /* freq_min */ 0.2, /* freq_max */ 15 }
};

zdev_t zio_motion1_device =
{
	"motion1", PIN_MOTION_1, 1,
	ZDEV_MOTION, DEVF_START | DEVF_INPUT, ZMOD_EXTERNAL,
	/* op */
	{ zio_motion_open, zio_motion_read, NULL, zio_motion_print, zio_motion_close, zio_motion_poll },
	/* param */
	{ /* freq_min */ 0.2, /* freq_max */ 15, 0, PIN_NULL }
};


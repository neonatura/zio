/* Copyright 2018 Neo Natura */

#include "zio.h"

#define DEFAULT_MOOD_PERCENT 50.0
 
int zio_mood_open(zdev_t *dev)
{

	zio_dev_on(dev);

	zio_dvalue_set(dev, DEFAULT_MOOD_PERCENT);

	return (0);
}
 
int zio_mood_read(zdev_t *dev)
{
	double d;

	d = zio_mod_dvalue_avg(dev);
	if (d == 0.00)
		return (ZERR_AGAIN); /* skip */

	/* store last obtained value. */
	zio_dvalue_set(dev, d);

	return (0);
}

int zio_mood_poll(zdev_t *dev)
{
	int err;
	if (is_zio_dev_on(dev)) {
		err = zio_mood_read(dev);
		if (err)
			return (err);
	}
	return (0);
}

int zio_mood_close(zdev_t *dev)
{
	zio_dev_off(dev);
	return (0);
}

int zio_mood_print(zdev_t *dev, int mode, void *retbuf)
{
	char buf[256];

	sprintf(retbuf, "%-2.2f%%", zio_dvalue_avg(dev, MAX_VALUE_DOUBLE_SIZE));

	return (0);
}

zdev_t zio_mood_device =
{
	"mood", PIN_NULL, 0, /* contoller: internal temperature */
	ZDEV_EMOTE, DEVF_START | DEVF_MODULE, ZMOD_INTERNAL, 
	/* op */
	{ zio_mood_open, zio_mood_read, NULL, zio_mood_print, zio_mood_close, zio_mood_poll },
	/* param */
	{ /* freq_min */ 150, /* freq_max */ 300, 0, PIN_NULL }
};



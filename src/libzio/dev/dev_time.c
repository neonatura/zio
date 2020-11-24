/* Copyright 2018 Neo Natura */

#include "zio.h"

int zio_time_open(zdev_t *dev)
{

	zio_dev_on(dev);

	return (0);
}
 
int zio_time_read(zdev_t *dev)
{
	uint64_t stamp;

	/* calculate average time. */
	stamp = zio_mod_time_avg(dev);

	/* store last obtained value. */
	zio_value_set(dev, stamp);

	return (0);
}

int zio_time_poll(zdev_t *dev)
{
	int err;
	if (is_zio_dev_on(dev)) {
		err = zio_time_read(dev);
		if (err)
			return (err);
	}
	return (0);
}

int zio_time_close(zdev_t *dev)
{
	zio_dev_off(dev);
	return (0);
}

int zio_time_print(zdev_t *dev, int mode, void *retbuf)
{
	char buf[256];
	ztime_t stamp;

	stamp = zio_value_get(dev);
	strftime(buf, sizeof(buf)-1, "%X %x", zio_localtime(stamp));

	strcpy((char *)retbuf, buf);
	return (0);
}

zdev_t zio_time_device =
{
	"time", PIN_NULL, 0, /* contoller: local time */
	ZDEV_TIME, DEVF_START | DEVF_MODULE, ZMOD_INTERNAL, 
	/* op */
	{ zio_time_open, zio_time_read, NULL, zio_time_print, zio_time_close, zio_time_poll },
	/* param */
	{ /* freq_min */ 60, /* freq_max */ 120, 0, PIN_NULL }
};


/* Copyright 2018 Neo Natura */

#include "zio.h"

int zio_itime_open(zdev_t *dev)
{

	zio_dev_on(dev);

	return (0);
}
 
int zio_itime_read(zdev_t *dev)
{
	ztime_t stamp;

	/* sample system internal time. */
	stamp = zio_time();
	if (stamp <= 1000)
		return (ERR_AGAIN);

	/* store last obtained value. */
	zio_value_set(dev, stamp);

	return (0);
}

int zio_itime_write(zdev_t *dev, uint8_t *data, size_t data_len)
{
	struct tm *tm;
	time_t stamp;

	if (data_len != sizeof(time_t))
		return (ERR_INVAL);

	memcpy(&stamp, data, sizeof(time_t));
	zio_time_set(stamp);
	return (0);
}

int zio_itime_poll(zdev_t *dev)
{
	int err;
	if (is_zio_dev_on(dev)) {
		err = zio_itime_read(dev);
		if (err)
			return (err);
	}
	return (0);
}

int zio_itime_close(zdev_t *dev)
{
	zio_dev_off(dev);
	return (0);
}

int zio_itime_print(zdev_t *dev, int mode, void *retbuf)
{
	char buf[256];
	ztime_t stamp; 

	stamp = zio_value_get(dev);
	strftime(buf, sizeof(buf)-1, "%X %x", zio_localtime(stamp));

	strcpy((char *)retbuf, buf);
	return (0);
}

zdev_t zio_itime_device =
{
	"itime", PIN_NULL, 1, /* contoller: local itime */
	ZDEV_TIME, DEVF_START, ZMOD_INTERNAL, 
	/* op */
	{ zio_itime_open, zio_itime_read, zio_itime_write, zio_itime_print, zio_itime_close, zio_itime_poll },
	/* param */
	{ /* freq_min */ 15, /* freq_max */ 45, 0, PIN_NULL }
};


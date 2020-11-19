/* Copyright 2018 Neo Natura */

#include "zio.h"

int zio_gtime_open(zdev_t *dev)
{
	int fd;

	if (is_zio_dev_on(dev))
		return (0);

	zio_dev_on(dev);

	return (0);
}

int zio_gtime_write(zdev_t *dev, uint8_t *data, size_t data_len)
{
	char buf[256];
	int len;
	int fd;

	if (!zio_dev_on(dev))
		return (ERR_INVAL);

	if (data_len == sizeof(uint64_t)) {
		uint64_t val = *((uint64_t *)data);
		zio_value_set(dev, val);
	}

	return (0);
}

int zio_gtime_close(zdev_t *dev)
{
	if (!zio_dev_on(dev))
		return;

	zio_dev_off(dev);
}

int zio_gtime_print(zdev_t *dev, int mode, void *retbuf)
{
        char buf[256];
        time_t stamp;

        stamp = zio_timeu(zio_value_get(dev) + zio_fifo_span(dev));
        strftime(buf, sizeof(buf)-1, "%X %x", localtime(&stamp));

        strcpy((char *)retbuf, buf);
        return (0);
}

int zio_gtime_poll(zdev_t *dev)
{
	return (0);
}

zdev_t zio_gtime_device =
{
        "gtime", PIN_NULL, STRATUM_LOW, /* contoller: gps time */
        ZDEV_TIME, DEVF_START, ZMOD_INTERNAL,
        /* op */
        { zio_gtime_open, NULL, zio_gtime_write, zio_gtime_print, zio_gtime_close, zio_gtime_poll },
        /* param */
        { /* freq_min */ 60, /* freq_max */ 600, 0, PIN_NULL }
};



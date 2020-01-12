/* Copyright 2018 Neo Natura */

#include "zio.h"
 
int zio_debug_open(zdev_t *dev)
{
	int fd;

	if (is_zio_dev_on(dev))
		return (0);

	zio_dev_on(dev);

	return (0);
}
 
int zio_debug_write(zdev_t *dev, uint8_t *data, size_t data_len)
{

	if (!is_zio_dev_on(dev))
		return (ZERR_AGAIN);

	zio_mod_write(dev, data, data_len);
	return (0);
}

int zio_debug_close(zdev_t *dev)
{

	if (!is_zio_dev_on(dev))
		return (0);

	zio_dev_off(dev);

	return (0);
}

zdev_t zio_debug_device =
{
	"debug", PIN_NULL, STRATUM_MAX, /* contoller: activity log */
	ZDEV_LOG, DEVF_START | DEVF_OUTPUT | DEVF_MODULE, ZMOD_INTERNAL, 
	/* op */
	{ zio_debug_open, NULL, zio_debug_write, NULL, zio_debug_close },
	/* param */
	{ /* freq_min */ 0.5, /* freq_max */ 5 }
};


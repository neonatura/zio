/* Copyright 2018 Neo Natura */

#include "zio.h"
 
int zio_debug_open(zdev_t *dev)
{
	int fd;

	if (is_zio_dev_on(dev))
		return (0);

	fd = open(SYS_DEBUG_PATH, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (fd == -1) {
		return (fd);
	}

	dev->dev_fd = fd;
	zio_dev_on(dev);

	return (0);
}
 
int zio_debug_write(zdev_t *dev, uint8_t *data, size_t data_len)
{
	char tbuf[256];
	time_t now;

	if (!is_zio_dev_on(dev))
		return (ZERR_AGAIN);

	now = time(NULL);
	memset(tbuf, 0, sizeof(tbuf));
	strftime(tbuf, sizeof(tbuf)-1, "%H:%M:%S ", localtime(&now));
	write(dev->dev_fd, tbuf, strlen(tbuf));

	write(dev->dev_fd, data, data_len);
	write(dev->dev_fd, "\n", 1);

	return (0);
}

int zio_debug_close(zdev_t *dev)
{

	if (!is_zio_dev_on(dev))
		return (0);

	close(dev->dev_fd);
	zio_dev_off(dev);

	return (0);
}

zdev_t zio_debug_device =
{
	"debug", PIN_NULL, STRATUM_MAX, /* contoller: activity log */
	ZDEV_LOG, DEVF_START | DEVF_OUTPUT, ZMOD_INTERNAL, 
	/* op */
	{ zio_debug_open, NULL, zio_debug_write, NULL, zio_debug_close },
	/* param */
	{ /* freq_min */ 0.5, /* freq_max */ 5 }
};


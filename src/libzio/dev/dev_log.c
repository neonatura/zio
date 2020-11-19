/* Copyright 2018 Neo Natura */

#include "zio.h"
 
int zio_log_open(zdev_t *dev)
{
	const char *log_path = zsys_log_path(); 
	char path[PATH_MAX+1];
	int fd;

	if (is_zio_dev_on(dev))
		return (0);

	sprintf(path, "%s/zio.log", log_path);
	fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (fd == -1) {
		return (fd);
	}

	dev->dev_fd = fd;
	zio_dev_on(dev);

	return (0);
}
 
int zio_log_write(zdev_t *dev, uint8_t *data, size_t data_len)
{
	char tbuf[256];
	time_t now;

	if (!is_zio_dev_on(dev))
		return (ERR_AGAIN);

	now = time(NULL);
	memset(tbuf, 0, sizeof(tbuf));
	strftime(tbuf, sizeof(tbuf)-1, "[%H:%M:%S] ", localtime(&now));
	write(dev->dev_fd, tbuf, strlen(tbuf));

	write(dev->dev_fd, data, data_len);
	write(dev->dev_fd, "\n", 1);

	return (0);
}

int zio_log_close(zdev_t *dev)
{

	if (!is_zio_dev_on(dev))
		return (0);

	close(dev->dev_fd);
	zio_dev_off(dev);

	return (0);
}

zdev_t zio_log_device =
{
	"log", PIN_NULL, STRATUM_MAX, /* contoller: activity log */
	ZDEV_DIAG, DEVF_START | DEVF_OUTPUT, ZMOD_INTERNAL, 
	/* op */
	{ zio_log_open, NULL, zio_log_write, NULL, zio_log_close },
	/* param */
	{ /* freq_min */ 1, /* freq_max */ 60, 0, PIN_NULL }
};


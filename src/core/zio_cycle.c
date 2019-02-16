/* Copyright 2019 Neo Natura */

#include "zio.h"

void zio_dev_cycle(zdev_t *dev)
{
	uint64_t now = zio_mtime();
	int err;

	if (now < dev->stat.freq_stamp) {
		return;
	}

	dev->stat.freq_stamp = now + dev->stat.freq;

	if (dev->op.timer) {
		err = (*dev->op.timer)(dev);
		if (!err) {
			zio_debug(dev);
		} else if (err != ZERR_AGAIN) {
			zio_error(dev, err, "timer");
		}
	}

	dev->stat.freq_cycle++;
}

void zio_cycle(void)
{
	zdev_t *dev;

	for (dev = zio_device_table; dev; dev = dev->next) {
		if (!is_zio_dev_on(dev))
			continue;

		zio_dev_cycle(dev);
	}

}


void zio_cycle_loop(void)
{
	while (1) {
		zio_msleep(1);
		zio_cycle();
	}
}


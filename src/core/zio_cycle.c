/* Copyright 2019 Neo Natura */

#include "zio.h"

uint64_t cycle_ticks;
uint64_t cycle_ops;

void zio_dev_cycle(zdev_t *dev)
{
	uint64_t now = zio_time();
	int err;

	if (now < dev->stat.freq_stamp) {
		return;
	}

	dev->stat.freq_stamp = now + dev->stat.freq;

	if (dev->op.timer) {
		err = (*dev->op.timer)(dev);
		if (!err) {
			if (dev->flags & DEVF_MODULE) {
				zio_notify(dev);
			} else {
				zio_debug(dev);
				if ((dev->type != ZDEV_EMOTE) &&
						!(dev->flags & DEVF_MODULE))
					zio_mood_incr(dev);
			}
			cycle_ops++;
		} else if (err != ZERR_AGAIN) {
			zio_error(dev, err, "timer");
			if ((dev->type != ZDEV_EMOTE) &&
					!(dev->flags & DEVF_MODULE))
				zio_mood_decr(dev);
		}
	}

	dev->stat.freq_cycle++;
}

void zio_cycle(void)
{
	zdev_t *dev;

	cycle_ticks++;

	for (dev = zio_device_table; dev; dev = dev->next) {
		if (!is_zio_dev_on(dev))
			continue;

		zio_dev_cycle(dev);
	}

}

void zio_cycle_step(void)
{
	zio_msleep(1);
	zio_cycle();
}

void zio_cycle_loop(void)
{
	while (1) {
		zio_cycle_step();
	}
}



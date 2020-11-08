/* Copyright 2018 Neo Natura */

#include "zio.h"

int zioctl(zdev_t *dev, int reg, int set, uint8_t *raw, size_t raw_len)
{
#if 0
	zioctl_t *ctl = (zioctl_t *)raw;
	uint8_t *data_raw = (raw + sizeof(zioctl_t));
	size_t data_len = raw_len - sizeof(zioctl_t);

	switch (ctl->reg) {
		case ZCTL_WAKE:
			if (!ctl->set)
				return (!(dev->flags & DEVF_SLEEP));
			return (zio_sgp30_wake(dev));
		case ZCTL_SLEEP:
			if (!ctl->set)
				return (dev->flags & DEVF_SLEEP);
			return (zio_sgp30_sleep(dev));
		case ZCTL_FREQ:
			break;
		case ZCTL_HUMIDITY:
			break;
	}
#endif
	return (0);
}

int zioctl_get(zdev_t *dev, int reg)
{
	return (zio_ioctl(dev, reg, TRUE, NULL, 0));
}


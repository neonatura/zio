/* Copyright 2018 Neo Natura */

#include "zio.h"

double zio_mod_dvalue_avg(zdev_t *mod)
{
	zdev_t *dev;
	double ret;
	double d;
	uint32_t ret_tot;
	uint32_t fact;

	ret = 0;
	ret_tot = 0;
	for (dev = zio_device_table; dev; dev = dev->next) {
		if (mod->module != dev->module)
			continue;
		if (mod->type != dev->type)
			continue;
		if (dev->flags & DEVF_MODULE)
			continue;

		d = zio_dvalue_avg(dev, MAX_VALUE_DOUBLE_SIZE-1);
		fact = MAX(0, 255 - dev->stratum);
		ret += (double)d * fact;
		ret_tot += fact;
	}
	if (ret_tot != 0)
		ret /= ret_tot;

	return (ret);
}


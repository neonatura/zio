/* Copyright 2018 Neo Natura */

#include "zio.h"
 
int zio_itemp_open(zdev_t *dev)
{

	zio_dev_on(dev);

	return (0);
}
 
int zio_itemp_read(zdev_t *dev)
{
	FILE *fl;
	char buf[16];

	fl = fopen(SYS_ITEMP_PATH, "r");
	if (!fl) {
		zio_dev_off(dev);
		return (ERR_INVAL);
	}

	memset(buf, 0, sizeof(buf));
	fgets(buf, sizeof(buf), fl);
	fclose(fl);

	if (!*buf) {
		return (ERR_INVAL);
	}

	/* store last obtained value. */
	zio_dvalue_set(dev, atof(buf) / 1000);

	return (0);
}

int zio_itemp_poll(zdev_t *dev)
{
	int err;
	if (is_zio_dev_on(dev)) {
		err = zio_itemp_read(dev);
		if (err)
			return (err);
	}
	return (0);
}

int zio_itemp_close(zdev_t *dev)
{
	zio_dev_off(dev);
	return (0);
}

int zio_itemp_print(zdev_t *dev, int mode, void *retbuf)
{
	char buf[256];

	if (mode == ZIO_FMT_FAHRENHEIT) {
		sprintf(retbuf, "%-2.2fF", zio_therm_f((double)zio_dvalue_avg(dev, 4)));
	} else if (mode == ZIO_FMT_KELVIN) {
		sprintf(retbuf, "%-2.2fK", zio_therm_k((double)zio_dvalue_avg(dev, 4)));
	} else /* mode == ZIO_FMT_CELSIUS */ {
		sprintf(retbuf, "%-2.2fC", (double)zio_dvalue_avg(dev, 4));
	}

	return (0);
}

zdev_t zio_itemp_device =
{
	"itemp", PIN_NULL, STRATUM_MID, /* contoller: internal temperature */
	ZDEV_THERM, DEVF_START | DEVF_INPUT, ZMOD_INTERNAL, 
	/* op */
	{ zio_itemp_open, zio_itemp_read, NULL, zio_itemp_print, zio_itemp_close, zio_itemp_poll },
	/* param */
	{ /* freq_min */ 5, /* freq_max */ 15, 0, PIN_NULL }
	//{ /* freq_min */ 30, /* freq_max */ 60, 0, PIN_NULL }
};


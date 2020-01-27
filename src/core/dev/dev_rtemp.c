/* Copyright 2018 Neo Natura */

#include "zio.h"

#define REG_GET(_dev, _reg) \
	bcdToDec(wiringPiI2CReadReg8((_dev)->dev_fd, _reg))

static uint8_t bcdToDec(uint8_t val)
{
  return( (val/16*10) + (val%16) );
}

int zio_rtemp_open(zdev_t *dev)
{
	dev->dev_fd = ZIO_I2C_INIT(DEFAULT_DS3231_I2C_ADDRESS); 
	zio_dev_on(dev);
	return (0);
}

double _rtemp_ds3231_temp_read(zdev_t *dev)
{
        unsigned int temp;

        temp = REG_GET(dev, 0x11) << 8;
        temp |= REG_GET(dev, 0x12);

        return ((double)temp/256);
}

int zio_rtemp_read(zdev_t *dev)
{
	/* store time value */
	zio_dvalue_set(dev, _rtemp_ds3231_temp_read(dev));
	return (0);
}

int zio_rtemp_poll(zdev_t *dev)
{
	int err;
	if (is_zio_dev_on(dev)) {
		err = zio_rtemp_read(dev);
		if (err)
			return (err);
	}
	return (0);
}

int zio_rtemp_close(zdev_t *dev)
{
	zio_dev_off(dev);
	return (0);
}

int zio_rtemp_print(zdev_t *dev, int mode, void *retbuf)
{
	char buf[256];

	if (mode == ZIO_FMT_FAHRENHEIT) {
		sprintf(retbuf, "%-2.2fF", zio_therm_f(zio_dvalue_avg(dev, 4)));
	} else if (mode == ZIO_FMT_KELVIN) {
		sprintf(retbuf, "%-2.2fK", zio_therm_k(zio_dvalue_avg(dev, 4)));
	} else /* mode == ZIO_FMT_CELSIUS */ {
		sprintf(retbuf, "%-2.2fC", zio_dvalue_avg(dev, 4));
	}

	return (0);
}

zdev_t zio_rtemp_device =
{
	"rtemp", PIN_NULL, STRATUM_MID, /* contoller: RTC clock temp */
	ZDEV_THERM, DEVF_START | DEVF_INPUT, ZMOD_INTERNAL,
	/* op */
	{ zio_rtemp_open, zio_rtemp_read, NULL, zio_rtemp_print, zio_rtemp_close, zio_rtemp_poll },
	/* param */
	{ /* freq_min */ 15, /* freq_max */ 90, 0, PIN_NULL }
};


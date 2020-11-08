/* Copyright 2018 Neo Natura */

#include "zio.h"

#define REG_GET(_dev, _reg) \
	bcdToDec(wiringPiI2CReadReg8((_dev)->dev_fd, _reg))

#define REG_SET(_dev, _reg, _val) \
	wiringPiI2CWriteReg8((_dev)->dev_fd, _reg, decToBcd(_val))

static uint8_t decToBcd(uint8_t val)
{
  return( (val/10*16) + (val%10) );
}

static uint8_t bcdToDec(uint8_t val)
{
  return( (val/16*10) + (val%16) );
}

int zio_rtc_open(zdev_t *dev)
{
	dev->dev_fd = ZIO_I2C_INIT(DEFAULT_DS3231_I2C_ADDRESS); 
	zio_dev_on(dev);
	return (0);
}

uint64_t _rtc_ds3231_read(zdev_t *dev)
{
	struct tm tm;

	memset(&tm, 0, sizeof(tm));
	tm.tm_sec = REG_GET(dev, 0x0);
	tm.tm_min = REG_GET(dev, 0x1);
	tm.tm_hour = REG_GET(dev, 0x2);
	tm.tm_mday = REG_GET(dev, 0x4);
	tm.tm_mon = REG_GET(dev, 0x5);
	tm.tm_year = REG_GET(dev, 0x6) + 100;

	/* parse UTC time. */
	return (zio_timelocal(&tm));
}

void _rtc_ds3231_write(zdev_t *dev, uint64_t stamp)
{
	struct tm *tm = zio_localtime(stamp);

	/* store UTC time */
	REG_SET(dev, 0x0, tm->tm_sec);
	REG_SET(dev, 0x1, tm->tm_min);
	REG_SET(dev, 0x2, tm->tm_hour);
	REG_SET(dev, 0x4, tm->tm_mday);
	REG_SET(dev, 0x5, tm->tm_mon);
	REG_SET(dev, 0x6, tm->tm_year - 100);
}

int zio_rtc_read(zdev_t *dev)
{
	uint64_t stamp;

	/* read time */
	stamp = _rtc_ds3231_read(dev);

	/* store time value */
	zio_value_set(dev, stamp);
	return (0);
}

int zio_rtc_write(zdev_t *dev, uint8_t *data, size_t data_len)
{
	uint64_t stamp;

	if (data_len != sizeof(uint64_t))
		return (ZERR_INVAL);

	memcpy(&stamp, data, sizeof(stamp));
	_rtc_ds3231_write(dev, stamp);

	return (0);
}

int zio_rtc_poll(zdev_t *dev)
{
	int err;
	if (is_zio_dev_on(dev)) {
		err = zio_rtc_read(dev);
		if (err)
			return (err);
	}
	return (0);
}

int zio_rtc_close(zdev_t *dev)
{
	zio_dev_off(dev);
	return (0);
}

int zio_rtc_print(zdev_t *dev, int mode, void *retbuf)
{
	char buf[256];
	time_t stamp;

	memset(buf, 0, sizeof(buf));
	stamp = zio_timeu(zio_value_get(dev));
	strftime(buf, sizeof(buf)-1, "%X %x", localtime(&stamp));

	strcpy((char *)retbuf, buf);
	return (0);
}

zdev_t zio_rtc_device =
{
	"rtc", PIN_NULL, 15, /* contoller: internal clock */
	ZDEV_TIME, DEVF_START | DEVF_INPUT, ZMOD_INTERNAL,
	/* op */
	{ zio_rtc_open, zio_rtc_read, NULL, zio_rtc_print, zio_rtc_close, zio_rtc_poll },
	/* param */
	{ /* freq_min */ 15, /* freq_max */ 60, 0, PIN_NULL }
};


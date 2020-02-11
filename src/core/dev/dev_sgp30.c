/* Copyright 2018 Neo Natura */

#include "zio.h"

static const uint8_t _init_air_quality[2] = {0x20, 0x03};
static const uint8_t _measure_air_quality[2] = {0x20, 0x08};
static const uint8_t _get_baseline[2] = {0x20, 0x15};
static const uint8_t _set_baseline[2] = {0x20, 0x1E};
static const uint8_t _set_humidity[2] = {0x20, 0x61};
static const uint8_t _get_feature_set_version[2] = {0x20, 0x2F};
static const uint8_t _get_serial_id[2] = {0x36, 0x82};
//static const uint8_t measure_test[2] = {0x20, 0x32};
//static const uint8_t measure_raw_signals[2] = {0x20, 0x50};

#define SGP30_I2C_ADDRESS 0x58

//#define SGP30_DEFAULT_HUMIDITY 50

#define MIN_SGP30_CO2 400
#define MAX_SGP30_CO2 5000
#define SGP30_CO2_AQ(_ppm) \
	(double)(100.0 / 4600.0 * (4600.0 - MAX(0, (double)(_ppm) - 400)))

#define MIN_SGP30_VOC 50
#define MAX_SGP30_VOC 325
#define SGP30_VOC_AQ(_ppb) \
	(double)(100.0 / 275.0 * (275.0 - MAX(0, (double)(_ppb) - 50)))

static double _sgp30_humidity;
static int _sgp_co2_ppm;
static int _sgp_voc_ppb;
static int _sgp30_baseline_co2 = 35221;
static int _sgp30_baseline_voc = 40352; 
static uint64_t _zio_sgp30_serial;
static uint64_t _zio_sgp30_version;

/*
 * Minimum: 400 ppm CO2(eq) / 0 ppb TVOC
 */

static uint8_t gencrc8(uint8_t *data, size_t len)
{
    uint8_t crc = 0xff;
    size_t i, j;
    for (i = 0; i < len; i++) {
        crc ^= data[i];
        for (j = 0; j < 8; j++) {
            if ((crc & 0x80) != 0)
                crc = (uint8_t)((crc << 1) ^ 0x31);
            else
                crc <<= 1;
        }
    }
    return crc;
}

int zio_sgp30_version(zdev_t *dev, uint64_t *ver_p)
{
	unsigned char buf[16];
	int err;

	err = write(dev->dev_fd, _get_feature_set_version, 2);
	if (err != 2)
		return (ZERR_INVAL);

	usleep(500);

	memset(buf, 0, sizeof(buf));
	err = read(dev->dev_fd, buf, 3);
	if (err != 3)
		return (ZERR_INVAL);
	if (gencrc8(buf, 2) != buf[2])
		return (ZERR_ILSEQ);

	*ver_p = ((uint64_t)buf[0] << 8) | (uint64_t)buf[1];

	return (0);
}

int zio_sgp30_serial(zdev_t *dev, uint64_t *ser_p)
{
	unsigned char buf[16];
	int err;

	err = write(dev->dev_fd, _get_serial_id, 2);
	if (err != 2)
		return (ZERR_INVAL);

	usleep(500);

	memset(buf, 0, sizeof(buf));
	err = read(dev->dev_fd, buf, 9);
	if (err != 9)
		return (ZERR_INVAL);

	if (gencrc8(buf, 2) != buf[2])
		return (ZERR_ILSEQ);
	if (gencrc8(buf+3, 2) != buf[5])
		return (ZERR_ILSEQ);
	if (gencrc8(buf+6, 2) != buf[8])
		return (ZERR_ILSEQ);

	uint64_t ser1 = ((uint64_t)buf[0] << 8) | (uint64_t)buf[1];
	uint64_t ser2 = ((uint64_t)buf[3] << 8) | (uint64_t)buf[4];
	uint64_t ser3 = ((uint64_t)buf[6] << 8) | (uint64_t)buf[7];
	*ser_p = (ser3 << 32) + (ser2 << 16) + ser1;

	return (0);
}

/* absolute_humidity:   u32 absolute humidity in mg/m^3 */
int zio_sgp30_abs_humidity(zdev_t *dev, uint16_t hum)
{
	char buf[6];
	int err;

	memcpy(buf, _set_humidity, 2);
	memcpy(buf + 2, &hum, 2);
	buf[4] = gencrc8(buf + 2, 2);

	err = write(dev->dev_fd, buf, 5);
	if (err != 5)
		return (ZERR_INVAL);

	return (0);
}

int zio_sgp30_humidity_set(zdev_t *dev, double per)
{
	uint16_t hum;

	if (per == 0.0)
		return;

	if (floor(per) == floor(_sgp30_humidity))
		return;

	_sgp30_humidity = per;

	/* convert from relative to absolute humidity */
	hum = (uint32_t)(184 * per);
	return (zio_sgp30_abs_humidity(dev, hum));
}

int zio_sgp30_baseline(zdev_t *dev, int *b_co2_p, int *b_voc_p)
{
	char buf[6];
	int err;

	err = write(dev->dev_fd, _get_baseline, 2);
	if (err != 2)
		return (ZERR_INVAL);

	/* 6 bytes, baselineCO2 data(MSB) / data(LSB) / Checksum / baselineTVOC data(MSB) / data(LSB) / Checksum */
	err = read(dev->dev_fd, buf, 6);
	if (err != 6)
		return (ZERR_INVAL);

	*b_co2_p = ((uint32_t)buf[0] << 8) | (uint32_t)buf[1];
	*b_voc_p = ((uint32_t)buf[3] << 8) | (uint32_t)buf[4];

	return (0);
}

void zio_sgp30_baseline_init(zdev_t *dev, uint16_t base_co2, uint16_t base_voc)
{

	char buf[8];

	memcpy(buf, _set_baseline, 2);
	buf[2] = base_co2 >> 8;
	buf[3] = base_co2 & 0xFF;
	buf[4] = gencrc8(buf + 2, 2);
	buf[5] = base_voc >> 8; 
	buf[6] = base_voc & 0xFF;
	buf[7] = gencrc8(buf + 5, 2);
	(void)write(dev->dev_fd, buf, 8);

}

int zio_sgp30_open(zdev_t *dev)
{
	char buf[128];
	uint64_t ser;
	uint64_t ver;
	int err;
	int fd;

	dev->dev_fd = ZIO_I2C_INIT(dev->def_pin);

	err = write(dev->dev_fd, _init_air_quality, 2);
	if (err != 2)
		return (ZERR_INVAL);

	zio_sgp30_baseline_init(dev, _sgp30_baseline_co2, _sgp30_baseline_voc); 
//	zio_sgp30_humidity_set(fd, SGP30_DEFAULT_HUMIDITY);

	err = zio_sgp30_version(dev, &ver);
	if (err)
		return (err);

	err = zio_sgp30_serial(dev, &ser);
	if (err)
		return (err);

	_zio_sgp30_version = ver;
	_zio_sgp30_serial = ser;
	zio_dev_on(dev);

	return (0);
}

void zio_sgp30_read_init(zdev_t *dev)
{
	(void)write(dev->dev_fd, _measure_air_quality, 2);
}

int zio_sgp30_read(zdev_t *dev)
{
	unsigned char buf[6];
	uint16_t co2;
	uint16_t voc;
	int err;

	err = read(dev->dev_fd, buf, 6);	
	if (err != 6)
		return (ZERR_INVAL);

	co2 = ((uint16_t)buf[0] << 8) | buf[1];
	zio_dvalue_set(dev, SGP30_CO2_AQ(co2));
	_sgp_co2_ppm = (int)co2;

	voc = ((uint16_t)buf[3] << 8) | buf[4];
	zio_dvalue_set(dev, SGP30_VOC_AQ(voc));
	_sgp_voc_ppb = (int)voc;

	return (0);
}

int zio_sgp30_poll(zdev_t *dev)
{
	int freq = dev->stat.freq_cycle % 90;

	if (!is_zio_dev_on(dev))
		return (ZERR_INVAL);

	if (45 == freq) {
		zio_sgp30_read_init(dev);
		return (ZERR_AGAIN);
	} 
	if (46 == freq) {
		return (zio_sgp30_read(dev));
	}

#if 0
	if (47 == freq) {
		zio_sgp30_humidity_set(dev->dev_fd, zio_dht_humidity());
		return (0);
	}
#endif

	return (ZERR_AGAIN);
}

int zio_sgp30_close(zdev_t *dev)
{

	if (!is_zio_dev_on(dev))
		return (0);

	if (dev->dev_fd != 0) {
		close(dev->dev_fd);
		dev->dev_fd = 0;
	}

	zio_dev_off(dev);
	return (0);
}

int zio_sgp30_print(zdev_t *dev, int mode, void *retbuf)
{
	sprintf(retbuf, "%-2.2f%%", zio_dvalue_avg(dev, MAX_VALUE_DOUBLE_SIZE));
	return (0);
}

int zio_sgp30_sleep(zdev_t *dev)
{
	int err;

	if (dev->flags & DEVF_SLEEP)
		return (0);


	/* retain baseline. */
	err = zio_sgp30_baseline(dev,
		&_sgp30_baseline_co2, &_sgp30_baseline_voc);
	if (err)
		return (err);

	dev->flags |= DEVF_SLEEP;
	return (0);
}

int zio_sgp30_wake(zdev_t *dev)
{
	
	if (!(dev->flags & DEVF_SLEEP))
		return (0);

	/* restore baseline before pause */
	zio_sgp30_baseline_init(dev,
		_sgp30_baseline_co2, _sgp30_baseline_voc); 

	dev->flags &= ~DEVF_SLEEP;
	return (0);
}

int zio_sgp30_ctl(zdev_t *dev, int reg, void *raw)
{
	uint8_t *data_raw = (uint8_t *)raw;
	int err;

	switch (reg) {
		case ZCTL_PING:
			{
				uint64_t ser = 0;

				err = zio_sgp30_serial(dev, &ser);
				if (err != 0 || ser == 0)
					return (ZERR_INVAL);
			}
			return (0);

		case ZCTL_SERIAL:
			return (_zio_sgp30_serial);

		case ZCTL_VERSION:
			return (_zio_sgp30_version);

		case ZCTL_RESET:
			err = zio_sgp30_close(dev);
			if (err)
				return (err);
			usleep(10);
			err = zio_sgp30_open(dev);
			if (err)
				return (err);
			break;

		case ZCTL_SLEEP:
			if (data_raw) {
				uint32_t val = *((uint32_t *)raw);
				if (val == 0)
					zio_sgp30_wake(dev);
				else
					zio_sgp30_sleep(dev);
			}
			return (!(dev->flags & DEVF_SLEEP));

		case ZCTL_HUMIDITY:
			if (data_raw) {
				uint32_t val = *((uint32_t *)raw);
				zio_sgp30_humidity_set(dev, (double)val);
			}
			return ((int)_sgp30_humidity);

		case ZCTL_BASELINE:
			if (data_raw) {
				uint32_t val = *((uint32_t *)raw);
				zio_sgp30_baseline_init(dev, (val & 0xFFFF), (val >> 16));
			}
			return ((int)(_sgp30_baseline_co2 + (_sgp30_baseline_voc << 16)));
	}

	return (0);
}

zdev_t zio_sgp30_device =
{
	"sgp", SGP30_I2C_ADDRESS, 1, /* air quality sensor */
	ZDEV_AIR, DEVF_START | DEVF_INPUT, ZMOD_INTERNAL,
	/* op */
	{ zio_sgp30_open, zio_sgp30_read, NULL, zio_sgp30_print, zio_sgp30_close, zio_sgp30_poll, zio_sgp30_ctl },
	/* param */
	{ /* freq_min */ 0.01, /* freq_max */ 0.012, 0, PIN_NULL }
};


/* Copyright 2018 Neo Natura */

#include "zio.h"

#define MAX_DHT_TIMINGS 85

static double _dht_temp;
static double _dht_humidity;
 
int zio_dht_open(zdev_t *dev)
{
	PIN_MODE(dev->def_pin, INPUT );
	zio_dev_on(dev);
	return (0);
}
 
int zio_dht_read(zdev_t *dev)
{
	int data[5] = { 0, 0, 0, 0, 0 };
	uint8_t laststate	= HIGH;
	uint8_t counter		= 0;
	uint8_t j			= 0, i;

	data[0] = data[1] = data[2] = data[3] = data[4] = 0;

	/* prepare to read the pin */
	PIN_MODE( dev->def_pin, INPUT );

	/* detect change and read data */
	for ( i = 0; i < MAX_DHT_TIMINGS; i++ ) {
		counter = 0;
		while (DIGITAL_READ( dev->def_pin ) == laststate) {
			counter++;
			delayMicroseconds( 1 );
			if ( counter == 128 ) {
				break;
			}
		}
		laststate = DIGITAL_READ( dev->def_pin );
		if ( counter == 128 )
			break;
 
		/* ignore first 3 transitions */
		if ( (i >= 4) && (i % 2 == 0) ) {
			/* shove each bit into the storage bytes */
			data[j / 8] <<= 1;
			if ( counter > 16 )
				data[j / 8] |= 1;
			j++;
		}

		if (j >= 40)
			break;
	}
 
	/*
	 * check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
	 * print it out if data is good
	 */
	if ( (j >= 40) &&
	     (data[4] == ( (data[0] + data[1] + data[2] + data[3]) & 0xFF) ) )
	{
		double h = (double)((data[0] << 8) + data[1]) / 10;
		if ( h > 100 )
		{
			h = data[0];	// for DHT11
		}
		double c = (double)(((data[2] & 0x7F) << 8) + data[3]) / 10;
		if ( c > 125 )
		{
			c = data[2];	// for DHT11
		}
		if ( data[2] & 0x80 )
		{
			c = -c;
		}

		/* retain for control registers. */
		_dht_temp = c;
		_dht_humidity = h;

		/* store last obtained value. */
		zio_dvalue_set(dev, c);
		return (0);
	} 

	return (ERR_INVAL);
}

int zio_dht_poll(zdev_t *dev)
{
	struct timeval tv;

	if (!is_zio_dev_on(dev))
		return (ERR_INVAL);

	if (511 == (dev->conf.freq_cycle % 1024)) {
		PIN_MODE( dev->def_pin, OUTPUT );
		DIGITAL_WRITE( dev->def_pin, LOW );
		return (ERR_AGAIN);
	} else if (512 == (dev->conf.freq_cycle % 1024)) {
		PIN_MODE( dev->def_pin, INPUT );
		return (zio_dht_read(dev));
	}

	return (ERR_AGAIN);
}

int zio_dht_print(zdev_t *dev, int mode, void *retbuf)
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

int zio_dht_close(zdev_t *dev)
{
	zio_dev_off(dev);
}

double zio_dht_humidity(void)
{
	return (_dht_humidity);
}

int zio_dht_ctl(zdev_t *dev, int reg, void *data)
{
	switch (reg) {
		case ZCTL_VERSION:
			return (11); /* "DHT11" */

		case ZCTL_TEMP:
			return (_dht_temp);

		case ZCTL_HUMIDITY:
			return (_dht_humidity);
	}
}

zdev_t zio_dht0_device =
{
        "dht0", PIN_DHT0, 5, /* contoller: internal temperature */
        ZDEV_THERM, DEVF_START | DEVF_INPUT, ZMOD_INTERNAL,
        /* op */
        { zio_dht_open, zio_dht_read, NULL, zio_dht_print, zio_dht_close, zio_dht_poll, zio_dht_ctl },
        /* param */
        { /* freq_min */ 0.015, /* freq_max */ 0.018, /* 15ms - 18ms */
	0.2 /* ma */, PIN_DHT0_PWR },
};

zdev_t zio_dht1_device =
{
        "dht1", PIN_DHT1, 5, /* contoller: extrrnal temperature */
        ZDEV_THERM, DEVF_START | DEVF_INPUT, ZMOD_EXTERNAL,
        /* op */
        { zio_dht_open, zio_dht_read, NULL, zio_dht_print, zio_dht_close, zio_dht_poll, zio_dht_ctl },
        /* param */
        { /* freq_min */ 0.015, /* freq_max */ 0.018, /* 15ms - 18ms */
	0.2, PIN_NULL },
};



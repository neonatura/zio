/* Copyright 2018 Neo Natura */

#include "zio.h"
 
 
void zio_dht_init(zdev_t *dev)
{
	zio_dev_on(dev);
}
 
void zio_dht_read(zdev_t *dev)
{
	int data[5] = { 0, 0, 0, 0, 0 };
	uint8_t laststate	= HIGH;
	uint8_t counter		= 0;
	uint8_t j			= 0, i;

	data[0] = data[1] = data[2] = data[3] = data[4] = 0;
 
	/* pull pin down for 18 milliseconds */
	DIGITAL_MODE( PIN_DHT_SIG, OUTPUT );
	DIGITAL_WRITE( PIN_DHT_SIG, LOW );
	delay( 18 );
 
	/* prepare to read the pin */
	DIGITAL_MODE( PIN_DHT_SIG, INPUT );
 
	/* detect change and read data */
	for ( i = 0; i < MAX_DHT_TIMINGS; i++ )
	{
		counter = 0;
		while ( DIGITAL_READ( PIN_DHT_SIG ) == laststate )
		{
			counter++;
			delayMicroseconds( 1 );
			if ( counter == 255 )
			{
				break;
			}
		}
		laststate = DIGITAL_READ( PIN_DHT_SIG );
		if ( counter == 255 )
			break;
 
		/* ignore first 3 transitions */
		if ( (i >= 4) && (i % 2 == 0) ) {
			/* shove each bit into the storage bytes */
			data[j / 8] <<= 1;
			if ( counter > 16 )
				data[j / 8] |= 1;
			j++;
		}
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

		/* store last obtained value. */
		dev->dvalue = (double *)dev->value;
		dev->dvalue[0] = c;
		dev->dvalue[1] = h;
	} else  {
		zio_error_incr();
	}

}

void zio_dht_timer(zdev_t *dev)
{
	if (is_zio_dev_on(dev))
		zio_dht_read(dev);
}

void zio_dht_term(zdev_t *dev)
{
	zio_dev_off(dev);
}

int zdev_temp_c(zdev_t *dev)
{
	return (dev->dvalue[0]);
}

int zdev_temp_f(zdev_t *dev)
{
	return (dev->dvalue[0] * 1.8 + 32);
}

int zio_dev_humidity(zdev_t *dev)
{
	return (dev->dvalue[1]);
}

 

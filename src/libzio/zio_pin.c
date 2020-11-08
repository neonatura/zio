/* Copyright 2018 Neo Natura */

#include "zio.h"

void zio_pin_on(int pin)
{
	PIN_MODE(pin, OUTPUT);
	DIGITAL_WRITE(pin, HIGH);
}

void zio_pin_off(int pin)
{
	PIN_MODE(pin, OUTPUT);
	DIGITAL_WRITE(pin, LOW);
}

double zio_analog_value(int code)
{
	return ( (double)code * 3.3 / 255 );
}

#if 0
int zio_analog_init(void)
{
	PCF8591_INIT();
}
#endif


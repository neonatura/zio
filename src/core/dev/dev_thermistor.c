/* Copyright 2018 Neo Natura */
 
#define BCOEFFICIENT 3950
#define NOMINAL_TEMPERATURE 25
#define NOMINAL_RESISTANCE 22000
#define SERIES_RESISTANCE 6700 

#include "zio.h"

int zio_thermistor_open(zdev_t *dev)
{
	PIN_MODE(dev->def_pin, INPUT );
	zio_dev_on(dev);
}

static double _calc_temp(double Resistance)
{
	double steinhart;

	steinhart = Resistance / NOMINAL_RESISTANCE; // (R/Ro)
	steinhart = log(steinhart); // ln(R/Ro)
	steinhart /= BCOEFFICIENT; // 1/B * ln(R/Ro)
	steinhart += 1.0 / (NOMINAL_TEMPERATURE + 273.15); // + (1/To)
	steinhart = 1.0 / steinhart; // Invert
	steinhart -= 273.15; // convert to C

	return (steinhart);
}

int zio_thermistor_read(zdev_t *dev)
{
	double volt;
	double temp;
	double val;
	double r;

	val = (double)analogRead(dev->def_pin);
	if (val == 0.0 || val == 255.0)
		return (ZERR_AGAIN);

	volt = 3.3 / 255 * val;

	r = (255.0 / (255 - val)) - 1 + 0.001;
	r = SERIES_RESISTANCE / r;

	/* store last obtained value. */
	zio_dvalue_set(dev, _calc_temp(r));

	return (0);
}

int zio_thermistor_poll(zdev_t *dev)
{
	int err;

	if (is_zio_dev_on(dev)) {
		err = zio_thermistor_read(dev);
		if (err)
			return (err);
	}

	return (0);
}

int zio_thermistor_print(zdev_t *dev, int mode, void *retbuf)
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

int zio_thermistor_close(zdev_t *dev)
{
	zio_dev_off(dev);
}

zdev_t zio_thermistor0_device =
{
        "atemp0", PIN_A0, 1, /* contoller: analog temperature */
        ZDEV_THERM, DEVF_START | DEVF_INPUT, ZMOD_INTERNAL,
        /* op */
        { zio_thermistor_open, zio_thermistor_read, NULL, zio_thermistor_print, zio_thermistor_close, zio_thermistor_poll },
        /* param */
        { /* freq_min */ 1, /* freq_max */ 10, 0.02, PIN_NULL }
};

zdev_t zio_thermistor1_device =
{
        "atemp1", PIN_A1, 1, /* contoller: analog temperature */
        ZDEV_THERM, DEVF_START | DEVF_INPUT, ZMOD_INTERNAL,
        /* op */
        { zio_thermistor_open, zio_thermistor_read, NULL, zio_thermistor_print, zio_thermistor_close, zio_thermistor_poll },
        /* param */
        { /* freq_min */ 1, /* freq_max */ 10, 0.02, PIN_NULL }
};

zdev_t zio_thermistor2_device =
{
        "atemp2", PIN_A2, 1, /* contoller: analog temperature */
        ZDEV_THERM, DEVF_START | DEVF_INPUT, ZMOD_INTERNAL,
        /* op */
        { zio_thermistor_open, zio_thermistor_read, NULL, zio_thermistor_print, zio_thermistor_close, zio_thermistor_poll },
        /* param */
        { /* freq_min */ 1, /* freq_max */ 10, 0.02, PIN_NULL }
};

zdev_t zio_thermistor3_device =
{
        "atemp3", PIN_A3, 1, /* contoller: analog temperature */
        ZDEV_THERM, DEVF_START | DEVF_INPUT, ZMOD_INTERNAL,
        /* op */
        { zio_thermistor_open, zio_thermistor_read, NULL, zio_thermistor_print, zio_thermistor_close, zio_thermistor_poll },
        /* param */
        { /* freq_min */ 1, /* freq_max */ 10, 0.02, PIN_NULL }
};


/* Copyright 2018 Neo Natura */

#include "zio.h"

#define HERTZ 0.16666667

zdev_t *zio_device_table;

int fmt_therm = ZIO_FMT_FAHRENHEIT;

#if 0
/* The devices available for use. */
zdev_t _zio_device_table[MAX_ZIO_DEVICES] = 
{

	{ "gps", MODF_START | MODF_INPUT, 
		zio_gps_init, zio_gps_read, zio_dev_null,
		zio_dev_null, zio_dev_null, zio_dev_null,
		PIN_NULL, ZIO_GPS_MIN_MS, ZIO_GPS_MAX_MS, ZIO_GPS_MW, 1 },

	{ "gyro", MODF_START | MODF_INPUT, 
		zio_gyro_init, zio_gyro_read, zio_dev_null,
		zio_dev_null, zio_dev_null, zio_dev_null,
		PIN_GYRO_PWR, ZIO_GYRO_MIN_MS, ZIO_GYRO_MAX_MS, ZIO_GYRO_MW, 2 },

	{ "itherm", MODF_START | MODF_INPUT, 
		zio_dht_init, zio_dht_read, zio_dev_null,
		zio_dht_timer, zio_dev_null, zio_dht_term,
		PIN_DHT_PWR, ZIO_DHT_MIN_MS, ZIO_DHT_MAX_MS, ZIO_DHT_MW, 1 },

	{ "therm", MODF_START | MODF_INPUT, 
		zio_itemp_init, zio_itemp_read, zio_dev_null,
		zio_itemp_timer, zio_dev_null, zio_itemp_term,
		PIN_NULL, ZIO_ITEMP_MIN_MS, ZIO_ITEMP_MAX_MS, ZIO_DHT_MW, 2 },

	{ "speaker", MODF_START | MODF_OUTPUT, 
		zio_speaker_init, zio_dev_null, zio_speaker_write,
		zio_speaker_timer, zio_dev_null, zio_speaker_term,
		PIN_NULL, ZIO_SPEAKER_MIN_MS, ZIO_SPEAKER_MAX_MS, ZIO_SPEAKER_MW }

};
zdev_t *zio_device(int dev_id)
{
	if (dev_id < 0 || dev_id >= MAX_ZIO_DEVICES)
		return (NULL);
	return (&_zio_device_table[dev_id]);
}
int zio_device_id(zdev_t *dev)
{
	int idx;
	for (idx = 0; idx < MAX_ZIO_DEVICES; idx++) {
		if (dev == &_zio_device_table[idx])
			return (idx);
	}
	return (-1);
}

zio_mod_t *zio_module(int mod_id)
{
	if (mod_id < 0 || mod_id >= MAX_ZIO_MODULES)
		return (NULL);
	return (&_zio_module_table[mod_id]);
}

int zio_mod_claim(zio_mod_t *mod, zdev_t *dev)
{

	if (!mod||!dev)
		return (ZERR_INVAL);

	dev->mod_claim = zio_device_id(dev);
	return (0);
}

const char *zio_mod_name(zio_mod_t *mod)
{
	if (!mod) return (NULL);
	return (mod->label);
}
#endif

int zio_mod_release(zdev_t *dev)
{

	if (!dev)
		return (ZERR_INVAL);

	dev->mod_claim = ZDEV_NULL;
	return (0);
}

const char *zio_dev_name(zdev_t *dev)
{
	if (!dev) return (NULL);
	return (dev->label);
}

int is_zio_dev_claim(zdev_t *dev)
{
	return (dev->mod_claim != ZDEV_NULL);
}

int zio_dev_on(zdev_t *dev)
{

	if (dev->module == ZMOD_NULL)
		return (ZERR_INVAL);

	if (dev->flags & DEVF_POWER)
		return (0); /* already on */

	if (dev->param.pin_pwr != PIN_NULL) {
		/* pull up designated power pin */
		zio_pin_on(dev->param.pin_pwr);
	}

	dev->flags |= DEVF_POWER;
}

int zio_dev_off(zdev_t *dev)
{

	if (dev->mod_claim == ZDEV_NULL)
		return (ZERR_INVAL);

	if (dev->param.pin_pwr != PIN_NULL) {
		/* pull down designated power pin */
		zio_pin_off(dev->param.pin_pwr);
	}

	dev->flags &= ~DEVF_POWER;
}

int is_zio_dev_on(zdev_t *dev)
{
	if (!dev) return (FALSE);
	return ( (dev->flags & DEVF_POWER) );
}

void zio_dvalue_set(zdev_t *dev, double dvalue)
{
	zio_fifo_t *fifo = &dev->fifo;
	int idx;

	fifo->dvalue = (double *)fifo->value;

	fifo->value_len++;
	idx = fifo->value_len % MAX_VALUE_DOUBLE_SIZE;
	fifo->dvalue[idx] = dvalue;

	*fifo->dvalue = dvalue;
}

void zio_ivalue_set(zdev_t *dev, uint32_t ivalue)
{
	zio_fifo_t *fifo = &dev->fifo;
	int idx;

	fifo->ivalue = (uint32_t *)fifo->value;

	fifo->value_len++;
	idx = fifo->value_len % MAX_VALUE_DOUBLE_SIZE;
	fifo->ivalue[idx] = ivalue;

	*fifo->ivalue = ivalue;
}

double zio_dvalue_get(zdev_t *dev)
{
	zio_fifo_t *fifo = &dev->fifo;

	if (!fifo->dvalue)
		return (0);

	return (*fifo->dvalue);
}

double zio_ivalue_get(zdev_t *dev)
{
	zio_fifo_t *fifo = &dev->fifo;

	if (!fifo->ivalue)
		return (0);

	return (*fifo->ivalue);
}

double zio_dvalue_avg(zdev_t *dev, int max_cycles)
{
	zio_fifo_t *fifo = &dev->fifo;
	double avg;
	int max;
	int idx;

	if (!fifo->dvalue)
		return (0);

	avg = 0;
	max_cycles = MIN(max_cycles, fifo->value_len % MAX_VALUE_DOUBLE_SIZE);
	for (idx = 0; idx <= max_cycles; idx++) {
		avg += fifo->dvalue[idx];
	}
	avg /= (max_cycles+1);

	return (avg);
}


int zio_dev_null(zdev_t *dev)
{
	return (0);
}


void zio_fifo_init(zdev_t *dev)
{
	dev->fifo.dvalue = (double *)dev->fifo.value;
	dev->fifo.ivalue = (uint32_t *)dev->fifo.value;
}

int zio_dev_init(zdev_t *dev)
{
	double freq;
	int err;

	freq = (dev->param.freq_min + dev->param.freq_max) / 2;
	dev->stat.freq = MAX(HERTZ, freq * 1000);
	dev->stat.freq_stamp = zio_mtime() + zio_dev_startup_wait(dev) * 1000;

	zio_fifo_init(dev);

	if (dev->flags & DEVF_START) {
		if (dev->op.init) {
			err = (*dev->op.init)(dev);
			if (err)
				return (err);
		}
	}

	return (0);
}

int zio_dev_register(zdev_t *dev)
{
	int err;

	dev->next = zio_device_table;
	zio_device_table = dev;

	dev->stat.freq = (dev->param.freq_min + dev->param.freq_max) / 2;
	dev->stat.freq = MAX(HERTZ, dev->stat.freq);

	return (zio_dev_init(dev));
}

zdev_t *zio_dev_get(int module, int type, int flags)
{
	zdev_t *dev;

	for (dev = zio_device_table; dev; dev = dev->next) {
		if (module != ZMOD_NULL && dev->module != module)
			continue;
		if (type != ZDEV_NULL && dev->type != type)
			continue;
		if (flags != 0 && !(flags & dev->flags))
			continue;

		/* matched criteria specified. */
		return (dev);
	}

	return (NULL);
}

void zio_debug(zdev_t *dev)
{
	zdev_t *debug;
	char buf[256];

	debug = zio_dev_get(ZMOD_INTERNAL, ZDEV_LOG, 0);
	if (!debug)
		return;

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "[%s] ", zio_dev_name(dev)); 
	if (dev->type == ZDEV_THERM) {
		(*dev->op.print)(dev, fmt_therm, buf + strlen(buf));
	} else {
		(*dev->op.print)(dev, 0, buf + strlen(buf));
	}
	zio_debug_write(debug, buf, strlen(buf));
}

void zio_error(zdev_t *dev, int err, char *tag)
{
	zdev_t *debug;
	char buf[256];

	debug = zio_dev_get(ZMOD_INTERNAL, ZDEV_LOG, 0);
	if (!debug)
		return;

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "[%s] %s: %s (code %d).", 
			zio_dev_name(dev), tag, strerror(-err), err); 

	(*dev->op.print)(dev, ZIO_FMT_FAHRENHEIT, buf);
	zio_debug_write(debug, buf, strlen(buf) + 1);
}


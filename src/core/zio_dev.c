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
	if (!dev) return ("!");
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

	if (!fifo->dvalue)
		return;

	idx = fifo->value_len % MAX_VALUE_DOUBLE_SIZE;
	fifo->value_len++;

	fifo->dvalue[idx] = dvalue;
	fifo->value_stamp = zio_time();
}

void zio_ivalue_set(zdev_t *dev, uint32_t ivalue)
{
	zio_fifo_t *fifo = &dev->fifo;
	int idx;

	if (!fifo->ivalue)
		return;

	idx = fifo->value_len % MAX_VALUE_I32_SIZE;
	fifo->value_len++;

	fifo->ivalue[idx] = ivalue;
	fifo->value_stamp = zio_time();
}

void zio_value_set(zdev_t *dev, uint64_t lvalue)
{
	zio_fifo_t *fifo = &dev->fifo;
	int idx;

	if (!fifo->lvalue)
		return;

	idx = fifo->value_len % MAX_VALUE_I64_SIZE;
	fifo->value_len++;

	fifo->lvalue[idx] = lvalue;
	fifo->value_stamp = zio_time();
}

double zio_dvalue_get(zdev_t *dev)
{
	zio_fifo_t *fifo = &dev->fifo;
	int idx;

	if (!fifo->dvalue)
		return (0);

	if (fifo->value_len == 0)
		return (0);

	idx = (fifo->value_len - 1);
	return (fifo->dvalue[idx]);
}

uint32_t zio_ivalue_get(zdev_t *dev)
{
	zio_fifo_t *fifo = &dev->fifo;
	int idx;

	if (!fifo->ivalue)
		return (0);

	if (fifo->value_len == 0)
		return (0);

	idx = (fifo->value_len - 1);
	return (fifo->ivalue[idx]);
}

uint64_t zio_value_get(zdev_t *dev)
{
	zio_fifo_t *fifo = &dev->fifo;
	int idx;

	if (!fifo->lvalue)
		return (0);

	if (fifo->value_len == 0)
		return (0);

	idx = (fifo->value_len - 1);
	return (fifo->lvalue[idx]);
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
	for (idx = 0; idx < max_cycles; idx++) {
		avg += fifo->dvalue[idx];
	}
	if (max_cycles != 0)
		avg /= max_cycles;

	return (avg);
}

uint8_t *zio_data(zdev_t *dev)
{
	return (dev->fifo.value);
}

int zio_dev_null(zdev_t *dev)
{
	return (0);
}


void zio_fifo_init(zdev_t *dev)
{
	dev->fifo.dvalue = (double *)dev->fifo.value;
	dev->fifo.ivalue = (uint32_t *)dev->fifo.value;
	dev->fifo.lvalue = (uint64_t *)dev->fifo.value;
}

int zio_dev_init(zdev_t *dev)
{
	double freq;
	int err;

	freq = (dev->param.freq_min + dev->param.freq_max) / 2;
	dev->stat.freq = MAX(HERTZ, freq * 1000);
	dev->stat.freq_stamp = zio_time() + zio_dev_startup_wait(dev) * 1000;

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

	err = zio_dev_init(dev);
	if (err) {
		zio_error(dev, err, "init");
		return (err);
	}

	zio_notify_text(dev, "init");
	return (0);
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

zdev_t *zio_dev_get_name(zdev_t *in_dev, const char *name)
{
	zdev_t *dev;

	for (dev = zio_device_table; dev; dev = dev->next) {
		if (in_dev->module != ZMOD_NULL && 
				dev->module != in_dev->module)
			continue;

		if (0 == strcasecmp(dev->label, name)) {
			/* matched criteria specified. */
			return (dev);
		}
	}

	return (NULL);
}

zdev_t *zio_mod_get(int module, int type)
{
	return (zio_dev_get(module, type, DEVF_MODULE));
}

void zio_print(zdev_t *dev, int format, char *ret_buf)
{
	if (!dev->op.print)
		return;
	(*dev->op.print)(dev, 0, ret_buf);
}

void zio_debug(zdev_t *dev)
{
	zdev_t *debug;
	char buf[256];
	char buf2[256];

	if (!dev->op.print)
		return; /* n/a */

	debug = zio_dev_get_name(dev, "log");
	if (!debug)
		return;

	memset(buf, 0, sizeof(buf));
	memset(buf2, 0, sizeof(buf2));
	if (dev->type == ZDEV_THERM)
		zio_print(dev, fmt_therm, buf2); 
	else
		zio_print(dev, 0, buf2); 
	sprintf(buf, "%s %s", zio_dev_name(dev), buf2); 
	zio_write(debug, buf, strlen(buf));
}

void zio_notify(zdev_t *dev)
{
	zdev_t *debug;
	char buf[256];
	char buf2[256];

	debug = zio_mod_get(ZMOD_INTERNAL, ZDEV_LOG);
	if (!debug)
		return;

	memset(buf, 0, sizeof(buf));
	memset(buf2, 0, sizeof(buf2));
	if (dev->type == ZDEV_THERM)
		zio_print(dev, fmt_therm, buf2); 
	else
		zio_print(dev, 0, buf2); 
	sprintf(buf, "%s %s", zio_dev_name(dev), buf2); 
	zio_write(debug, buf, strlen(buf));
}

void zio_notify_text(zdev_t *dev, char *text)
{
	zdev_t *debug;
	char buf[256];

	debug = zio_mod_get(ZMOD_INTERNAL, ZDEV_LOG);
	if (!debug)
		return;

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%s %s", zio_dev_name(dev), text);
	zio_write(debug, buf, strlen(buf));
}

int zio_write(zdev_t *dev, uint8_t *data, size_t data_len)
{
	char tbuf[256];
	time_t now;
	int err;

	if (!is_zio_dev_on(dev))
		return (ZERR_AGAIN);


        if (!dev->op.write)
		return (ZERR_INVAL);

	err = (*dev->op.write)(dev, data, data_len);
	if (err)
		return (err);

	return (0);
}

int zio_write16_r(zdev_t *dev, uint8_t *data, size_t data_len, size_t rep_len)
{
	uint16_t *raw = (uint16_t *)data;
	size_t of;
	int err;
	int i;

	if (!is_zio_dev_on(dev))
		return (ZERR_AGAIN);

fprintf(stderr, "DEBUG: zio_write16_r: <%d bytes>\n", rep_len * data_len);

        if (!dev->op.write)
		return (ZERR_INVAL);

	for (of = 0; of < data_len; of += 2) {
		for (i = 0; i < rep_len; i++) {
			err = (*dev->op.write)(dev, &data[of], 2);
			if (err)
				return (err);
		}
	}

	return (0);
}

int zio_write_r(zdev_t *dev, uint8_t *data, size_t data_len, int rep_no)
{
	char tbuf[256];
	time_t now;
	size_t len;
	int err;
	int i;

	if (!is_zio_dev_on(dev))
		return (ZERR_AGAIN);


        if (!dev->op.write)
		return (ZERR_INVAL);

	for (len = 0; len < data_len; len++) {
		for (i = 0; i < rep_no; i++) {
			err = (*dev->op.write)(dev, &data[len], 1);
			if (err)
				return (err);
		}
	}

	return (0);
}

void zio_error(zdev_t *dev, int err, char *tag)
{
	zdev_t *debug;
	char buf[256];

	debug = zio_dev_get_name(dev, "log");
	if (!debug)
		return;

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%s %s: %s (%d).", 
			zio_dev_name(dev), tag, strerror(-err), err); 
	zio_write(debug, buf, strlen(buf));
}

uint64_t zio_fifo_span(zdev_t *dev)
{
	if (!dev)
		return (0);
	return (zio_time() - dev->fifo.value_stamp);
}

int zio_digital_read(int pin)
{
	if (pin >= 80 && pin <= 95) {
		zio_sc16is_pin_get(pin);
		return;
	}

#ifdef HAVE_LIBWIRINGPI
	return (digitalRead(pin));
#endif

	return (0);
}

void zio_digital_write(int pin, int val)
{

	if (pin >= 80 && pin <= 95) {
		zio_sc16is_pin_set(pin, val);
		return;
	}

#ifdef HAVE_LIBWIRINGPI
	digitalWrite(pin, val);
#endif
}

void zio_digital_mode(int pin, int mode)
{

	if (pin >= 80 && pin <= 95) {
		zio_sc16is_pin_mode(pin, mode);
		return;
	}

#ifdef HAVE_LIBWIRINGPI
	pinMode(pin, mode);
#endif
}


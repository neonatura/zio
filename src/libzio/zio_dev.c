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
		return (ERR_INVAL);

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
		return (ERR_INVAL);

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
		return (ERR_INVAL);

	if (dev->flags & DEVF_POWER)
		return (0); /* already on */

	if (dev->param.pin_pwr != PIN_NULL) {
		/* pull up designated power pin */
		zio_pin_on(dev->param.pin_pwr);
	}

	dev->flags |= DEVF_POWER;
	return (0);
}

int zio_dev_off(zdev_t *dev)
{

	if (dev->mod_claim == ZDEV_NULL)
		return (ERR_INVAL);

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

#if 0
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
#endif
void zio_dvalue_set(zdev_t *dev, num_t dvalue)
{
	int idx;

	idx = (dev->fifo.fifo_idx % dev->fifo.fifo_max);
	dev->fifo.fifo_idx++;

	quat_set(dvalue, Q_NUM, dev->fifo.fifo[idx]);
	dev->fifo.fifo_time = ztime();
}

#if 0 
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
#endif
void zio_ivalue_set(zdev_t *dev, uint32_t ivalue)
{
	int idx;

	idx = dev->fifo.fifo_idx % dev->fifo.fifo_max;
	dev->fifo.fifo_idx++;

	quat_set((num_t)ivalue, Q_NUM, dev->fifo.fifo[idx]);
	dev->fifo.fifo_time = ztime();
}

#if 0
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
#endif
void zio_value_set(zdev_t *dev, uint64_t lvalue)
{
	int idx;

	idx = dev->fifo.fifo_idx % dev->fifo.fifo_max;
	dev->fifo.fifo_idx++;

	quat_set((num_t)lvalue, Q_NUM, dev->fifo.fifo[idx]);
	dev->fifo.fifo_time = ztime();
}

#if 0
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
#endif
num_t zio_dvalue_get(zdev_t *dev)
{
	int fifo_idx;

	if (dev->fifo.fifo_idx == 0) {
		return (0);
	}

	fifo_idx = (((uint)dev->fifo.fifo_idx - 1) % dev->fifo.fifo_max);

	{
		double f = quat_get(dev->fifo.fifo[fifo_idx]);
	}

	return (quat_get(dev->fifo.fifo[fifo_idx]));
}

#if 0
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
#endif
uint32_t zio_ivalue_get(zdev_t *dev)
{
	int idx;

	if (dev->fifo.fifo_idx == 0)
		return (0);

	idx = ((dev->fifo.fifo_idx - 1) % dev->fifo.fifo_max);

	return ((uint32_t)quat_get(dev->fifo.fifo[idx]));
}

#if 0
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
#endif
uint64_t zio_value_get(zdev_t *dev)
{
	int idx;

	if (dev->fifo.fifo_idx == 0)
		return (0);

	idx = ((dev->fifo.fifo_idx - 1) % dev->fifo.fifo_max);

	return ((uint64_t)quat_get(dev->fifo.fifo[idx]));
}

ztime_t zio_fifo_time(zdev_t *dev)
{
	return ((ztime_t)dev->fifo.fifo_time);
}

num_t zio_dvalue_avg(zdev_t *dev, uint max_cycles)
{
	num_t avg;
	int fifo_idx;
	int max;
	int idx;

	avg = 0;
	fifo_idx = (dev->fifo.fifo_idx % dev->fifo.fifo_max);
	max_cycles = MIN(max_cycles, fifo_idx);
	if (max_cycles == 0)
		return (0);

	max = 0;
	for (idx = (fifo_idx-1); idx >= 0; idx--) {
		avg += quat_get(dev->fifo.fifo[idx]);

		max++;
		if (max >= max_cycles)
			break;
	}
	if (max != 0)
		avg /= max;

	return (avg);
}

#if 0
uint8_t *zio_data(zdev_t *dev)
{
	return (dev->fifo.value);
}
int zio_data_ln(zdev_t *dev, uint8_t *data, size_t data_len)
{
	int idx;

	memset(data, 0, data_len);

	idx = stridx(dev->fifo.value, '\n');
	if (idx == -1)
		return (ERR_AGAIN);

	if (idx >= data_len)
		return (ERR_OVERFLOW);

	/* extract to return buffer */
	memcpy(data, dev->fifo.value, idx);

	/* remove from device buffer */
	idx++;
	memmove(dev->fifo.value, dev->fifo.value + idx, dev->fifo.value_len - idx);
	dev->fifo.value_len -= idx;

	return (0);
}
#endif
int zio_data_pull(zdev_t *dev, uint8_t *retdata, size_t retdata_len)
{
	qvar var;
	off_t of;
	int fifo_idx;
	int i;

	fifo_idx = (dev->fifo.fifo_idx % dev->fifo.fifo_max);
	if (fifo_idx == 0)
		return (-1);

	var = dev->fifo.fifo[0];
	of = quat_stream(var, retdata, retdata_len);
	/* note: if redata_len is too small, data is lost (fix me) */

	for (i = 1; i < fifo_idx; i++) {
		dev->fifo.fifo[i-1] = dev->fifo.fifo[i];
	}
	dev->fifo.fifo[i] = var;
	dev->fifo.fifo_idx = (fifo_idx - 1);

	return (of);
}

int zio_data_peek(zdev_t *dev, zaddr_t addr, uint8_t *retdata, size_t retdata_len)
{
	qvar var;
	int fifo_idx;
	off_t of;
	int i;

	fifo_idx = (dev->fifo.fifo_idx % dev->fifo.fifo_max);
	if (addr >= fifo_idx)
		return (-1);

	var = dev->fifo.fifo[addr];
	of = quat_stream(var, retdata, retdata_len);

	return (of);
}

#if 0
void zio_data_append(zdev_t *dev, uint8_t *data, size_t data_len)
{

	data_len = MIN(data_len, MAX_VALUE_BUFFER_SIZE - dev->fifo.value_len);
	memcpy(dev->fifo.value + dev->fifo.value_len, data, data_len); 
	dev->fifo.value_len += data_len;

}
#endif
void zio_data_append(zdev_t *dev, uint8_t *data, size_t data_len)
{
	qvar var;
	int idx;

	idx = (dev->fifo.fifo_idx % dev->fifo.fifo_max);
	dev->fifo.fifo_idx++;

	if (dev->fifo.fifo[idx])
		quat_free(&dev->fifo.fifo[idx]);

	dev->fifo.fifo[idx] = quat_alloc(Q_ARRAY, data, data_len);
	dev->fifo.fifo_time = ztime();
}

int zio_dev_null(zdev_t *dev)
{
	return (0);
}

#if 0
void zio_fifo_init(zdev_t *dev)
{
	dev->fifo.dvalue = (double *)dev->fifo.value;
	dev->fifo.ivalue = (uint32_t *)dev->fifo.value;
	dev->fifo.lvalue = (uint64_t *)dev->fifo.value;
}
#endif
void zio_fifo_init(zio_fifo_t *fifo, size_t buff_size)
{
	static num_t blank_num;
	int i;

	fifo->fifo = (qvar *)calloc(buff_size, sizeof(qvar));
	if (fifo->fifo) {
		fifo->fifo_max = buff_size;

		for (i = 0; i < buff_size; i++) {
			fifo->fifo[i] = quat_alloc(Q_NUM, (uint8_t *)&blank_num, sizeof(num_t)); 
		}
	}

	fifo->fifo_idx = 0;
}

void zio_fifo_free(zio_fifo_t *fifo)
{
	int i;
	
	if (fifo->fifo) {
		for (i = 0; i < fifo->fifo_max; i++) {
			quat_free(&fifo->fifo[i]);
		}
		free(fifo->fifo);
		fifo->fifo = NULL;
	}

	fifo->fifo_max = 0;
	fifo->fifo_idx = 0;
}

int zio_dev_init(zdev_t *dev)
{
	double freq;
	int err;

	freq = (dev->param.freq_min + dev->param.freq_max) / 2;
	dev->conf.freq = MAX(HERTZ, freq * 1000);
	dev->conf.freq_stamp = zio_time() + zio_dev_startup_wait(dev) * 1000;

	zio_fifo_init(&dev->fifo, MAX_VALUE_BUFFER_SIZE);

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
	char buf[256];
	int32_t ver;
	int32_t ser;
	int err;

	dev->next = zio_device_table;
	zio_device_table = dev;

	dev->conf.freq = (dev->param.freq_min + dev->param.freq_max) / 2;
	dev->conf.freq = MAX(HERTZ, dev->conf.freq);

	err = zio_dev_init(dev);
	if (err) {
		if (dev->type != ZDEV_DIAG)
			zio_dev_error(dev, err, "init");
		return (err);
	}

	ver = zio_ctl(dev, ZCTL_VERSION, NULL);
	ser = zio_ctl(dev, ZCTL_SERIAL, NULL);
	strcpy(buf, "init");
	if (ver > 0)
		sprintf(buf + strlen(buf), " v%u", ver);
	if (ser > 0)
		sprintf(buf + strlen(buf), " #%u", ser);
	zio_notify_text(dev, buf);

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
		if (in_dev) {
			if (in_dev->module != ZMOD_NULL && 
					dev->module != in_dev->module)
				continue;
		}

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

int zio_print(zdev_t *dev, int format, char *ret_buf)
{
	int err;

	if (!dev->op.print)
		return (0);

	return ( (*dev->op.print)(dev, 0, ret_buf) );
}

void zio_dev_log(zdev_t *dev)
{
	zdev_t *log;
	char buf[256];
	char buf2[256];
	int err;

	if (!dev->op.print)
		return; /* n/a */

	log = zio_dev_get_name(dev, "log");
	if (!log)
		return;

	memset(buf, 0, sizeof(buf));
	memset(buf2, 0, sizeof(buf2));
	if (dev->type == ZDEV_THERM)
		err = zio_print(dev, fmt_therm, buf2); 
	else
		err = zio_print(dev, 0, buf2); 
	if (err)
		return;

	memset(buf, 0, sizeof(buf));
	snprintf(buf, sizeof(buf)-1, "%s %s", zio_dev_name(dev), buf2); 
	zio_write(log, buf, strlen(buf));
}

void zio_notify(zdev_t *dev)
{
	zdev_t *debug;
	char buf[256];
	char buf2[256];
	int err;

	debug = zio_mod_get(ZMOD_INTERNAL, ZDEV_DIAG);
	if (!debug)
		return;

	memset(buf, 0, sizeof(buf));
	memset(buf2, 0, sizeof(buf2));

	if (dev->type == ZDEV_THERM)
		err = zio_print(dev, fmt_therm, buf2); 
	else
		err = zio_print(dev, 0, buf2); 
	if (err)
		return;

	sprintf(buf, "%s %s", zio_dev_name(dev), buf2); 
	zio_write(debug, buf, strlen(buf));
}

void zio_notify_text(zdev_t *dev, char *text)
{
	zdev_t *debug;
	char buf[256];

	debug = zio_mod_get(ZMOD_INTERNAL, ZDEV_DIAG);
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
		return (ERR_AGAIN);

	if (!dev->op.write)
		return (ERR_INVAL);

	if (!data || data_len == 0)
		return (0);

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
		return (ERR_AGAIN);

        if (!dev->op.write)
		return (ERR_INVAL);

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
		return (ERR_AGAIN);


        if (!dev->op.write)
		return (ERR_INVAL);

	for (len = 0; len < data_len; len++) {
		for (i = 0; i < rep_no; i++) {
			err = (*dev->op.write)(dev, &data[len], 1);
			if (err)
				return (err);
		}
	}

	return (0);
}

void zio_dev_error(zdev_t *dev, int err, char *tag)
{
	zdev_t *error;
	char buf[256];

	error = zio_mod_get(ZMOD_INTERNAL, ZDEV_DIAG);
	//error = zio_dev_get_name(dev, "log");
	if (!error)
		return;

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%s %s: %s (%d).", 
			zio_dev_name(dev), tag, strerror(-err), err); 
	zio_write(error, buf, strlen(buf));
}

void zio_dev_debug(zdev_t *dev, int err, char *tag)
{
	zdev_t *debug;
	char buf[256];

	debug = zio_dev_get_name(dev, "log");
	if (!debug)
		return;

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%s %s: %s (%d).", 
			zio_dev_name(dev), tag, zstrerror(err), err); 
	zio_write(debug, buf, strlen(buf));
}

uint64_t zio_fifo_span(zdev_t *dev)
{
	if (!dev)
		return (0);
	return ((uint64_t)(ztime() - dev->fifo.fifo_time));
}

int zio_digital_read(int pin)
{
	if (pin >= 80 && pin <= 95) {
		zio_sc16is_pin_get(pin);
		return (0);
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

int zio_uart_init(zdev_t *dev, int chan, int baud)
{

	if (chan == 0) {
		int fd;

#ifdef HAVE_LIBWIRINGPI
		fd = serialOpen("/dev/ttyAMA0", baud);
#else
		fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_SYNC);
		/*
		 * set_interface_attribs (fd, B115200, 0);  // set speed to 115,200 bps, 8n1 (no parity)
set_blocking (fd, 0);                // set no blocking
*/
#endif
		return (fd);
	}

#ifdef HAVE_LIBWIRINGPI /* SC16IS */
	dev->flags |= DEVF_UART;
	/* previously uart channel, and now i2c address. 
	dev->def_pin = 80 + ((chan-1) * 8);
	return (0); */
	return (80 + ((chan-1) * 8));
#else
	return (-1);
#endif
}

int zio_uart_read(zdev_t *dev, uint8_t *retdata, size_t retdata_len)
{
	zdev_t *uart_dev;
	size_t data_len;
	int of;

	if (!(dev->flags & DEVF_UART)) {
		int ch;
		int of;

		if (dev->dev_fd == 0)
			return (ERR_INVAL);

		of = 0;
		while (of < retdata_len) {
#ifdef HAVE_LIBWIRINGPI
			ch = serialGetchar(dev->dev_fd);
#else
			{
				static char buf[4];
				int err;

				ch = -1;
				err = read(dev->dev_fd, buf, 1);
				if (err == 1)
					ch = buf[0];
			}
#endif
			if (ch == -1)
				break;

			retdata[of] = (uint8_t)ch;
			of++;	
		}

		return (of);
	}

	uart_dev = zio_sc16is_dev(dev->dev_fd);
	if (!uart_dev)
		return (ERR_INVAL);

#if 0
	data_len = uart_dev->fifo.value_len;
	for (of = 0; of < retdata_len && of < data_len; of++) {
		retdata[of] = uart_dev->fifo.value[of];
	}
	if (of != 0) {
		memmove(uart_dev->fifo.value, uart_dev->fifo.value + of,
			uart_dev->fifo.value_len - of);
		uart_dev->fifo.value_len -= of;
	}
#endif
	of = zio_data_pull(uart_dev, retdata, retdata_len);

	return (of);
}

zgeo_t *zio_geo_value(zdev_t *dev)
{
	static zgeo_t geo;
	num_t val[4];
	uint fifo_idx;
	uint idx;
	int i;

	fifo_idx = (dev->fifo.fifo_idx % dev->fifo.fifo_max);
	if (fifo_idx <= 4)
		return (NULL);

	for (i = 0; i < 4; i++) {
		idx = (fifo_idx - 4 + i);
		val[idx] = quat_get(dev->fifo.fifo[idx]);
	}
	geo.lat = (double)val[0];
	geo.lon = (double)val[1];
	geo.alt = (double)val[2];
	geo.stamp = (uint64_t)val[3];

	return (&geo);
}

/* positively influence emotional mood. */
void zio_mood_incr(zdev_t *dev)
{
	zdev_t *mod;
	double per;

	mod = zio_dev_get_name(dev, "mood");
	if (!mod)
		return;

	per = zio_dvalue_get(mod);
	per = MIN(99.9, per + 0.1);

	zio_dvalue_set(mod, per);
}

/* negatively influence emotional mood. */
void zio_mood_decr(zdev_t *dev)
{
	zdev_t *mod;
	double per;

	mod = zio_dev_get_name(dev, "mood");
	if (!mod)
		return;

	per = zio_dvalue_get(mod);
	per = MAX(0.1, per - 0.1);

	zio_dvalue_set(mod, per);
}

int zio_ctl(zdev_t *dev, int reg, void *data)
{

	if (dev->op.ctl) {
		return ( (*dev->op.ctl)(dev, reg, data) );
	}

	return (ERR_OPNOTSUPP);
}

void zio_init(void)
{

	/* log module */
	REGISTER_LOG_DEVICE();
	REGISTER_DIAG_DEVICE();

	/* time module */
	REGISTER_ITIME_DEVICE();
	REGISTER_TIME_DEVICE();

	/* therm module */
//	REGISTER_DUMMY_TEMP_DEVICE();
	REGISTER_ITEMP_DEVICE();
//	REGISTER_THERM_DEVICE();

}

void zio_term(void)
{
	zdev_t *dev;

	for (dev = zio_device_table; dev; dev = dev->next) {
		(*dev->op.term)(dev);
	}
}


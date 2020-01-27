/* Copyright 2020 Neo Natura */

#include "zio.h"

void zio_led_sleep(zdev_t *dev)
{

	if (dev->flags & DEVF_SLEEP)
		return;
	
	zio_pin_off(dev->def_pin);
	dev->flags |= DEVF_SLEEP;
}

void zio_led_wake(zdev_t *dev)
{

	if (!(dev->flags & DEVF_SLEEP))
		return;

	softPwmCreate(dev->def_pin, 0, 32);
	dev->flags &= ~DEVF_SLEEP;
}

int zio_led_open(zdev_t *dev)
{
	int err;
	int fd;

	if (is_zio_dev_on(dev))
		return (0);

	zio_dev_on(dev);

	zio_led_sleep(dev);

	return (0);
}
 
int zio_led_write(zdev_t *dev, uint8_t *data, size_t data_len)
{
	int i;

	if (!is_zio_dev_on(dev))
		return (ZERR_AGAIN);

	memset(dev->fifo.value, 0, MAX_VALUE_BUFFER_SIZE);
	strncpy(dev->fifo.value, data, MIN(data_len, MAX_VALUE_BUFFER_SIZE-1));
	dev->fifo.value_stamp = zio_time();

	zio_led_wake(dev);

	return (0);
}

int zio_led_close(zdev_t *dev)
{

	if (!is_zio_dev_on(dev))
		return (0);

	zio_pin_off(dev->def_pin);
	zio_dev_off(dev);

	return (0);
}

int zio_led_poll(zdev_t *dev)
{
	uint64_t diff;

	diff = zio_fifo_span(dev);
	if (diff >= 2000) {
		/* "sleep mode" */
		zio_led_sleep(dev);
		return (ZERR_AGAIN);
	}

	zio_led_wake(dev);

	if (diff >= 1000) {
		diff = 1000 - (diff - 1000);
	}
	diff /= 30;
	diff = MIN(32, diff);

	softPwmWrite(dev->def_pin, (int)diff);
	return (0);
}

zdev_t zio_led0_device =
{
	"led0", PIN_LED0, STRATUM_MAX, /* send notification */
	ZDEV_LOG, DEVF_START | DEVF_OUTPUT, ZMOD_INTERNAL, 
	/* op */
	{ zio_led_open, NULL, zio_led_write, NULL, zio_led_close, zio_led_poll },
	/* param */
	{ /* freq_min */ 0.06, /* freq_max */ 0.12, 0.002, PIN_NULL }
};

zdev_t zio_led1_device =
{
	"led1", PIN_LED1, STRATUM_MAX, /* send notification */
	ZDEV_LOG, DEVF_START | DEVF_OUTPUT, ZMOD_EXTERNAL, 
	/* op */
	{ zio_led_open, NULL, zio_led_write, NULL, zio_led_close, zio_led_poll },
	/* param */
	{ /* freq_min */ 0.06, /* freq_max */ 0.12, 0.002, PIN_NULL }
};


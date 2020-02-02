/* Copyright 2018 Neo Natura */

#include "zio.h"

#define MAX_ZIO_BUFFER_SIZE 1000
static uint16_t _zio_speaker_buffer[MAX_ZIO_BUFFER_SIZE];
static unsigned int _zio_speaker_buffer_index;
static int _zio_speaker_pin;

void zio_speaker_intr(void)
{
	uint16_t note;

	note = _zio_speaker_buffer[_zio_speaker_buffer_index % MAX_ZIO_BUFFER_SIZE];
	_zio_speaker_buffer[_zio_speaker_buffer_index % MAX_ZIO_BUFFER_SIZE] = 0;
	_zio_speaker_buffer_index++;

	ZIO_TONE(_zio_speaker_pin, note);
}

int zio_speaker_open(zdev_t *dev)
{

	if (is_zio_dev_on(dev))
		return (0);

	ZIO_TONE_INIT(dev->def_pin);
	ZIO_TONE(dev->def_pin, 0);

	/* setup PWM1 at 5kHZ */
	pinMode(PIN_PWM1, PWM_OUTPUT);
	pwmSetMode(PWM_MODE_MS);
	pwmSetClock(240);
	pwmSetRange(16);
	pwmWrite(PIN_PWM1, 1); 
	pwiringPiLSR(PIN_PWM1, INT_EDGE_FALLING, zio_speaker_intr); 

	zio_dev_on(dev);

	return (0);
}

int zio_speaker_write(zdev_t *dev, uint8_t *data, size_t data_len)
{
	unsigned char *raw;
	size_t len;

	raw = (unsigned char *)dev->fifo.value + dev->fifo.value_len;
	data_len = MIN(data_len, 
		(MAX_VALUE_BUFFER_SIZE - dev->fifo.value_len));

	memcpy(raw, data, data_len);
	dev->fifo.value_len += data_len;

	return (0);
}

/* called every 200ms */
int zio_speaker_poll(zdev_t *dev)
{
	static uint64_t last_t;
	uint64_t t;
	uint16_t sample;
	int len;
	int data_len;
	int idx;
	int of;

	if (!is_zio_dev_on(dev))
		return (ZERR_INVAL);

	of = _zio_speaker_buffer_index + 1;
	data_len = MIN(dev->fifo.value_len, MAX_ZIO_BUFFER_SIZE);
	for (len = 0; len < data_len; len++) {
		_zio_speaker_buffer[of % MAX_ZIO_BUFFER_SIZE] = dev->fifo.value[len];
		of++;
	}

	/* copy new second of audio content into buffer. */
	if (dev->fifo.value_len > len)
		memmove(dev->fifo.value, dev->fifo.value + len, dev->fifo.value_len - len);
	dev->fifo.value_len -= len;

	return (0);
}

int zio_speaker_close(zdev_t *dev)
{

	/* turn off speaker */
	ZIO_TONE_TERM(dev->def_pin);
	pinMode(dev->def_pin, OUTPUT);
	DIGITAL_WRITE(dev->def_pin, LOW);	

	/* turn off pwm1 */
	pinMode(PIN_PWM1, OUTPUT);
	DIGITAL_WRITE(PIN_PWM1, LOW);	

	zio_dev_off(dev);
}

zdev_t zio_speaker_device =
{
	"speaker", PIN_SPEAKER, 0, /* contoller: local speaker */
	ZDEV_AUDIO, DEVF_START, ZMOD_INTERNAL,
	/* op */
	{ zio_speaker_open, NULL, zio_speaker_write, NULL, zio_speaker_close, zio_speaker_poll },
	/* param */
	{ /* freq_min */ 0.2, /* freq_max */ 0.2, 1, PIN_NULL }
};


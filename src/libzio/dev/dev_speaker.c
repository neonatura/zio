/* Copyright 2018 Neo Natura */

#include "zio.h"
#include <signal.h>

#define MAX_ZIO_BUFFER_SIZE 1000
static uint16_t _zio_speaker_buffer[MAX_ZIO_BUFFER_SIZE];
static uint32_t _zio_speaker_buffer_index;
static int _zio_speaker_pin;

void zio_speaker_intr(int _unused)
{
	static uint16_t l_note;
	const int note_idx = _zio_speaker_buffer_index % MAX_ZIO_BUFFER_SIZE;

	_zio_speaker_buffer_index++;

	if (l_note != _zio_speaker_buffer[note_idx]) {
		ZIO_TONE(_zio_speaker_pin, _zio_speaker_buffer[note_idx]);
fprintf(stdout, "%u ", (unsigned int)_zio_speaker_buffer[note_idx]);
		l_note = _zio_speaker_buffer[note_idx];
	}

	_zio_speaker_buffer[note_idx] = 0;
}

int zio_speaker_open(zdev_t *dev)
{
	int err;

	if (is_zio_dev_on(dev))
		return (0);

	ZIO_TONE_INIT(dev->def_pin);
	ZIO_TONE(dev->def_pin, 0);
	_zio_speaker_pin = dev->def_pin;

	err = zio_speaker_sleep(dev);
	if (err)
		return (err);

	zio_dev_on(dev);

	return (0);
}

int zio_speaker_wake(zdev_t *dev)
{

	if (!(dev->flags & DEVF_SLEEP))
		return (0);

	/* set repeating alarm at 5kHZ. */
	signal(SIGALRM, zio_speaker_intr);
	ualarm(200, 200);

	dev->flags &= ~DEVF_SLEEP;

	return (0);
}

int zio_speaker_sleep(zdev_t *dev)
{

	if (dev->flags & DEVF_SLEEP)
		return (0);

	/* unset alarm signal */
	ualarm(0, 0);
	signal(SIGALRM, SIG_IGN);

	dev->flags |= DEVF_SLEEP;

	return (0);
}

int zio_speaker_write(zdev_t *dev, uint8_t *data, size_t data_len)
{
	unsigned char *raw;
	size_t len;
	int err;

	raw = (unsigned char *)dev->fifo.value + dev->fifo.value_len;
	data_len = MIN(data_len, 
		(MAX_VALUE_BUFFER_SIZE - dev->fifo.value_len));
	if (data_len == 0)
		return (ERR_OVERFLOW);

	err = zio_speaker_wake(dev);
	if (err)
		return (err);

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
		return (ERR_INVAL); /* incorrect state */

	if (dev->flags & DEVF_SLEEP)
		return (0); /* all done */

	of = _zio_speaker_buffer_index;
	data_len = MIN(dev->fifo.value_len, MAX_ZIO_BUFFER_SIZE);
	for (len = 0; len < data_len; len += 2) {
		memcpy(&_zio_speaker_buffer[of % MAX_ZIO_BUFFER_SIZE], dev->fifo.value + len, 2);
		of++;
	}
	if (len == 0) {
		(void)zio_speaker_sleep(dev);
		return (ERR_AGAIN);
	}

	/* copy new 200ms segment of audio content into buffer. */
	memmove(dev->fifo.value, dev->fifo.value + len, dev->fifo.value_len - len);
	dev->fifo.value_len -= len;

	return (0);
}

int zio_speaker_close(zdev_t *dev)
{

	(void)zio_speaker_sleep(dev);

	/* turn off speaker */
	ZIO_TONE_TERM(dev->def_pin);
	pinMode(dev->def_pin, OUTPUT);
	DIGITAL_WRITE(dev->def_pin, LOW);	

	zio_dev_off(dev);
}

zdev_t zio_speaker_device =
{
	"speaker", PIN_SPEAKER, 1, /* contoller: local speaker */
	ZDEV_AUDIO, DEVF_START, ZMOD_INTERNAL,
	/* op */
	{ zio_speaker_open, NULL, zio_speaker_write, NULL, zio_speaker_close, zio_speaker_poll },
	/* param */
	{ /* freq_min */ 0.2, /* freq_max */ 0.2, 1, PIN_NULL }
};


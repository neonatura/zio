/* Copyright 2018 Neo Natura */

#include "zio.h"

static unsigned char _zio_speaker_buffer[ZIO_AUDIO_FREQ];

int zio_speaker_init(zdev_t *dev)
{
	ZIO_TONE_INIT(PIN_SPEAKER_OUT);
}

int zio_speaker_write(zdev_t *dev, uint16_t *data, size_t data_len)
{
	unsigned char *raw;

	raw = (unsigned char *)dev->value;
	
	data_len = MIN(data_len, (MAX_VALUE_BUFFER_SIZE - dev->value_len) / 2);
	memcpy((uint16_t *)raw, data, data_len);
	dev->value_len += data_len;

	zio_dev_on(dev);
	zio_timer_incr(dev);
}

void zio_speaker_timer(zdev_t *dev)
{
	static const double freq = 1000 / ZIO_AUDIO_FREQ;
	static uint64_t last_t;
	uint64_t t = zio_mtime();
	uint16_t sample;
	int len;
	int idx;

	if (!is_zio_dev_on(dev))
		return;

	if ((t/1000) != last_t) {
		if (dev->value_of == 0) {
			zio_speaker_term(dev);
			return;
		}
		last_t = (t/1000);

		len = MIN(ZIO_AUDIO_FREQ, dev->value_of);
		memset(_zio_speaker_buffer, 0, ZIO_AUDIO_FREQ);
		memcpy(_zio_speaker_buffer, dev->value, len);
		if (dev->value_of > len)
			memmove(dev->value, dev->value + len, dev->value_of - len);
		dev->value_of -= len;
	}

	idx = ((uint32_t)(t / freq) % ZIO_AUDIO_FREQ);
	sample = _zio_speaker_buffer[idx];
	if (sample == NOTE_NONE) {
		ZIO_TONE(PIN_SPEAKER_OUT, 0);
	} else {
		ZIO_TONE(PIN_SPEAKER_OUT, sample);
	}

}

void zio_speaker_term(zdev_t *dev)
{
	ZIO_TONE(PIN_SPEAKER_OUT, 0);
	zio_timer_decr(dev);
	zio_dev_off(dev);
}


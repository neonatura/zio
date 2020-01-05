/* Copyright 2018 Neo Natura */

#include "zio.h"

static unsigned char _zio_audio_buffer[ZIO_AUDIO_FREQ];

#define BEEP_SAMPLES 14
static const uint16_t _beep_samples[BEEP_SAMPLES] = {
	NOTE_B4, NOTE_B4, 0, 0, NOTE_B4, NOTE_B4,
	NOTE_A4, NOTE_A4, 0, 0, NOTE_A4, NOTE_A4,
	NOTE_D5, NOTE_D5
};


#define INTRO_SAMPLES 38
static const uint16_t _intro_samples[INTRO_SAMPLES] = {
	NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4,
	0, 0,

	NOTE_AS4, NOTE_AS4, NOTE_AS4,
	NOTE_A3, NOTE_A3, NOTE_A3, NOTE_A3, NOTE_A3, NOTE_A3,
	0, 0,

	NOTE_G3, NOTE_G3,NOTE_G3,
	NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4,
	0, 0,

	NOTE_A5, NOTE_A5, NOTE_A5, NOTE_A5, NOTE_A5, NOTE_A5,
	0, 0,
};

int zio_audio_init(zdev_t *dev)
{
	ZIO_TONE_INIT(PIN_AUDIO_OUT);
}

int zio_audio_write(zdev_t *dev, uint16_t *data, size_t data_len)
{
	unsigned char *raw;

	raw = (unsigned char *)dev->fifo.value;
	
	data_len = MIN(data_len, (MAX_VALUE_BUFFER_SIZE - dev->fifo.value_len) / 2);
	memcpy((uint16_t *)raw, data, data_len);
	dev->fifo.value_len += data_len;

	zio_dev_on(dev);
	zio_timer_incr(dev);
}

int zio_audio_timer(zdev_t *dev)
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
		if (dev->fifo.value_of == 0) {
			zio_audio_term(dev);
			return;
		}
		last_t = (t/1000);

		len = MIN(ZIO_AUDIO_FREQ, dev->fifo.value_of);
		memset(_zio_audio_buffer, 0, ZIO_AUDIO_FREQ);
		memcpy(_zio_audio_buffer, dev->fifo.value, len);
		if (dev->fifo.value_of > len)
			memmove(dev->fifo.value, dev->fifo.value + len, dev->fifo.value_of - len);
		dev->fifo.value_of -= len;
	}

	idx = ((uint32_t)(t / freq) % ZIO_AUDIO_FREQ);
	sample = _zio_audio_buffer[idx];
	if (sample == NOTE_NONE) {
		ZIO_TONE(PIN_AUDIO_OUT, 0);
	} else {
		ZIO_TONE(PIN_AUDIO_OUT, sample);
	}

}

int zio_audio_term(zdev_t *dev)
{
	ZIO_TONE(PIN_AUDIO_OUT, 0);
	zio_timer_decr(dev);
	zio_dev_off(dev);
}

int zio_audio_write_intro(zdev_t *dev)
{
	return (zio_audio_write(dev, (uint16_t *)_intro_samples, INTRO_SAMPLES));
}

int zio_audio_beep(zdev_t *dev)
{
	return (zio_audio_write(dev, (uint16_t *)_beep_samples, BEEP_SAMPLES));
}


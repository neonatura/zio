/* Copyright 2018 Neo Natura */

#include "zio.h"

static uint16_t _zio_speaker_buffer[ZIO_AUDIO_FREQ];

int zio_speaker_open(zdev_t *dev)
{

	if (is_zio_dev_on(dev))
		return (0);

//	pinMode(dev->def_pin, OUTPUT);
	ZIO_TONE_INIT(dev->def_pin);
	ZIO_TONE(dev->def_pin, 0);

	zio_dev_on(dev);

	return (0);
}

int zio_speaker_write_index(void)
{
	static const double freq = 1000 / ZIO_AUDIO_FREQ;
	uint64_t t = zio_time();
	double d; 
	int idx;

	d = (double)t/1000 - floor(t/1000);
	idx = ((uint32_t)((d*1000) / freq) % ZIO_AUDIO_FREQ);

	return (idx);
}

void zio_speaker_write_padd(zdev_t *dev)
{
	if (dev->fifo.value_len == 0) {
		uint8_t *raw = (uint8_t *)dev->fifo.value + dev->fifo.value_len;
		int idx = zio_speaker_write_index();
		idx *= 2;
		memset(raw, 0, idx);
		dev->fifo.value_len += idx;
	}
}

int zio_speaker_write(zdev_t *dev, uint8_t *data, size_t data_len)
{
	unsigned char *raw;

	zio_speaker_write_padd(dev);

	raw = (unsigned char *)dev->fifo.value + dev->fifo.value_len;
	data_len = MIN(data_len, 
		(MAX_VALUE_BUFFER_SIZE - dev->fifo.value_len));

	memcpy(raw, data, data_len);
	dev->fifo.value_len += data_len;

	return (0);
}

int zio_speaker_poll(zdev_t *dev)
{
	static const double freq = 1000 / ZIO_AUDIO_FREQ;
	static uint64_t last_t;
	uint64_t t;
	uint16_t sample;
	int len;
	int idx;

	if (!is_zio_dev_on(dev))
		return (ZERR_INVAL);

	t = zio_time();


	if ((t/1000) != last_t) {
		if (dev->fifo.value_len == 0) {
			ZIO_TONE(dev->def_pin, 0);
			memset(dev->fifo.value, 0, MAX_VALUE_BUFFER_SIZE);
			return (ZERR_AGAIN);
		}

		last_t = (t/1000);

		/* copy new second of audio content into buffer. */
		len = MIN(ZIO_AUDIO_FREQ * 2, dev->fifo.value_len);
		memset(_zio_speaker_buffer, 0, ZIO_AUDIO_FREQ * 2);
		memcpy(_zio_speaker_buffer, dev->fifo.value, len);
		if (dev->fifo.value_len > len)
			memmove(dev->fifo.value, dev->fifo.value + len, dev->fifo.value_len - len);
		dev->fifo.value_len -= len;
	}

	idx = zio_speaker_write_index();
	sample = _zio_speaker_buffer[idx];
	ZIO_TONE(dev->def_pin, sample);

	return (0);
}

int zio_speaker_close(zdev_t *dev)
{

	ZIO_TONE_TERM(dev->def_pin);

	pinMode(dev->def_pin, OUTPUT);
	DIGITAL_WRITE(dev->def_pin, LOW);	

#if 0
	zio_timer_decr(dev);
#endif

	zio_dev_off(dev);
}

zdev_t zio_speaker_device =
{
        "speaker", PIN_SPEAKER, 0, /* contoller: local speaker */
        ZDEV_AUDIO, DEVF_START, ZMOD_INTERNAL,
        /* op */
        { zio_speaker_open, NULL, zio_speaker_write, NULL, zio_speaker_close, zio_speaker_poll },
        /* param */
        { /* freq_min */ 0.03125, /* freq_max */ 0.03125, 1, PIN_NULL }
};


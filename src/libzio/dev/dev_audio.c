/* Copyright 2018 Neo Natura */

#include "zio.h"

int zio_audio_open(zdev_t *dev)
{

	if (is_zio_dev_on(dev))
					return (0);

	zio_dev_on(dev);

        zio_audio_intro();

	return (0);
}

int zio_audio_write(zdev_t *dev, uint8_t *data, size_t data_len)
{

	if (!is_zio_dev_on(dev))
		return (ERR_AGAIN);

	zio_mod_write(dev, data, data_len);

	return (0);
}

int zio_audio_close(zdev_t *dev)
{
	zio_dev_off(dev);
}

zdev_t zio_audio_device =
{
        "audio", PIN_NULL, 0, /* contoller: local audio */
        ZDEV_AUDIO, DEVF_START | DEVF_MODULE, ZMOD_INTERNAL,
        /* op */
        { zio_audio_open, NULL, zio_audio_write, NULL, zio_audio_close },
        /* param */
        { /* freq_min */ 1, /* freq_max */ 60, 0, PIN_NULL }
};



/* Copyright 2018 Neo Natura */

#ifndef __DEV_AUDIO_H__
#define __DEV_AUDIO_H__

#define DEV_AUDIO_FREQ 32

#define AUDIO_DEVICE() ((zdev_t *)(&zio_audio_device))
#define REGISTER_AUDIO_DEVICE() (zio_dev_register(AUDIO_DEVICE()))

extern zdev_t zio_audio_device;

int zio_audio_open(zdev_t *dev);

int zio_audio_write(zdev_t *dev, uint8_t *data, size_t data_len);

int zio_audio_poll(zdev_t *dev);

int zio_audio_close(zdev_t *dev);

#endif /* ndef __DEV_AUDIO_H__ */



/* Copyright 2018 Neo Natura */

#ifndef __DEV_SPEAKER_H__
#define __DEV_SPEAKER_H__

#define REGISTER_SPEAKER_DEVICE() (zio_dev_register(&zio_speaker_device))

extern zdev_t zio_speaker_device;

int zio_speaker_open(zdev_t *dev);

int zio_speaker_write(zdev_t *dev, uint8_t *data_in, size_t data_len);

int zio_speaker_poll(zdev_t *dev);

int zio_speaker_close(zdev_t *dev);

/*
#define ZIO_SPEAKER_MIN_MS 24 
#define ZIO_SPEAKER_MAX_MS 40
#define ZIO_SPEAKER_MW 1
*/

#endif /* ndef __DEV_SPEAKER_H__ */


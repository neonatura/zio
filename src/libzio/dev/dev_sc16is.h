
/* Copyright 2018 Neo Natura */

#ifndef __DEV_SC16IS_H__
#define __DEV_SC16IS_H__

#define REGISTER_SC16IS_4C_DEVICE() \
	(zio_dev_register(&zio_sc16is_4c_device))
#define REGISTER_UART1_DEVICE REGISTER_SC16IS_4C_DEVICE

#define REGISTER_SC16IS_4D_DEVICE() \
	(zio_dev_register(&zio_sc16is_4d_device))
#define REGISTER_UART2_DEVICE REGISTER_SC16IS_4D_DEVICE

extern zdev_t zio_sc16is_4c_device;
extern zdev_t zio_sc16is_4d_device;

int zio_sc16is_pin_get(int pin);

void zio_sc16is_pin_set(int pin, int state);

void zio_sc16is_pin_mode(int pin, int mode);

int zio_sc16is_open(zdev_t *dev);

int zio_sc16is_close(zdev_t *dev);

int zio_sc16is_read(zdev_t *dev);

int zio_sc16is_write(zdev_t *dev, uint8_t *data, size_t data_len);

zdev_t *zio_sc16is_dev(int pin);

#endif /* ndef __DEV_SC16IS_H__ */


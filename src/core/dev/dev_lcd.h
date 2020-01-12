/* Copyright 2018 Neo Natura */

#ifndef __DEV_LCD_H__
#define __DEV_LCD_H__

extern zdev_t zio_lcd_device;

#define REGISTER_LCD_DEVICE() (zio_dev_register(&zio_lcd_device))

#endif /* ndef __DEV_LCD_H__ */


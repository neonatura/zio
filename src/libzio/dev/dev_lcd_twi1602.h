/* Copyright 2018 Neo Natura */

#ifndef __DEV_LCD_TWI1602_H__
#define __DEV_LCD_TWI1602_H__

extern zdev_t zio_lcd_twi1602_device;

#define REGISTER_LCD_TWI1602_DEVICE() (zio_dev_register(&zio_lcd_twi1602_device))

#endif /* ndef __DEV_LCD_TWI1602_H__ */


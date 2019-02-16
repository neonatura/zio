/* Copyright 2018 Neo Natura */

#ifndef __ZIO_DEV_H__
#define __ZIO_DEV_H__

#define PCF8591_HID 0x48

#define ANALOG_PIN_BASE 0x64 /* 100 */


extern zdev_t *zio_device_table;


void zio_dvalue_set(zdev_t *dev, double dvalue);

double zio_dvalue_get(zdev_t *dev);

double zio_dvalue_avg(zdev_t *dev);

int zio_dev_register(zdev_t *dev);

/** Turn on a device. */
int zio_dev_on(zdev_t *dev);

/** Turn off a device. */
int zio_dev_off(zdev_t *dev);

/** A device operation which does nothing. */
int zio_dev_null(zdev_t *dev);

zdev_t *zio_dev_get(int module, int type, int flags);

void zio_debug(zdev_t *dev);

void zio_error(zdev_t *dev, int err, char *tag);



/** The time (in milliseconds) to wait before accessing a device. */
#define zio_dev_startup_wait(_dev) \
	((_dev)->param.freq_max * 2)


#ifdef USE_WIRINGPI

#define ANALOG_READ(_pin) analogRead(ANALOG_PIN_BASE+(_pin))
#define ANALOG_WRITE(_pin, _val) analogWrite(ANALOG_PIN_BASE+(_pin), (_val))
#define DIGITAL_READ(_pin) digitalRead(_pin)
#define DIGITAL_WRITE(_pin, _val) digitalWrite((_pin), (_val))
#define DIGITAL_MODE(_pin, _mode) digitalMode((_pin), (_val))
#define ZIO_I2C_INIT(_addr) wiringPiI2CSetup(_addr)
#define ZIO_I2C_READ(_dev) wiringPiI2CRead((_dev)->dev_fd)
#define ZIO_I2C_WRITE(_dev, _val) wiringPiI2CWrite((_dev)->dev_fd, (_val))
#define ZIO_I2C_REG_READ(_dev, _reg) wiringPiI2CReadReg8((_dev)->dev_fd, (_reg))
#define ZIO_I2C_REG_WRITE(_dev, _reg, _val) wiringPiI2CWriteReg8((_dev)->dev_fd, (_reg), (_val))
#define ZIO_I2C_REG16_READ(_dev, _reg) wiringPiI2CReadReg16((_dev)->dev_fd, (_reg))
#define ZIO_I2C_REG16_WRITE(_dev, _reg, _val) wiringPiI2CWriteReg16((_dev)->dev_fd, (_reg), (_val))
#define PCF8591_INIT() pcf8591Setup(ANALOG_PIN_BASE, 0x48) 

#else /* !WIRINGPI */

#define LOW 0
#define HIGH 1
#define INPUT 0 
#define OUTPUT 1

#define ANALOG_READ(_pin)
#define ANALOG_WRITE(_pin, _val)
#define DIGITAL_READ(_pin)
#define DIGITAL_WRITE(_pin, _val)
#define DIGITAL_MODE(_pin, _mode)
#define ZIO_I2C_INIT(_addr)
#define ZIO_I2C_READ(_dev)
#define ZIO_I2C_WRITE(_dev, _val)
#define ZIO_I2C_REG_READ(_dev, _reg)
#define ZIO_I2C_REG_WRITE(_dev, _reg, _val)
#define ZIO_I2C_REG16_READ(_dev, _reg)
#define ZIO_I2C_REG16_WRITE(_dev, _reg, _val)
#define PCF8591_INIT() ZERR_INVAL

#endif




#endif /* ndef __ZIO_DEV_H__ */


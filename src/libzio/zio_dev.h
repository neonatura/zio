/* Copyright 2018 Neo Natura */

#ifndef __ZIO_DEV_H__
#define __ZIO_DEV_H__

#ifdef HAVE_LIBWIRINGPI
#include <softTone.h>
#endif

#define PCF8591_HID 0x48

#define ANALOG_PIN_BASE 0x64 /* 100 */

#define DEFAULT_PCF8591_ADDRESS 0x48

#define DEFAULT_DS3231_I2C_ADDRESS 0x68

/* 16x2 */
/* #define DEFAULT_LCD_I2C_ADDR 0x27 */
/* 128x64 */
#define DEFAULT_LCD_I2C_ADDR 0x3c

#define STRATUM_MIN 0
#define STRATUM_LOW 63
#define STRATUM_MID 127
#define STRATUM_HIGH 191
#define STRATUM_MAX 255

extern zdev_t *zio_device_table;


void zio_dvalue_set(zdev_t *dev, double dvalue);

void zio_ivalue_set(zdev_t *dev, uint32_t ivalue);

void zio_value_set(zdev_t *dev, uint64_t lvalue);

double zio_dvalue_get(zdev_t *dev);

uint32_t zio_ivalue_get(zdev_t *dev);

uint64_t zio_value_get(zdev_t *dev);

double zio_dvalue_avg(zdev_t *dev, int max_cycles);

int zio_dev_register(zdev_t *dev);

/** Turn on a device. */
int zio_dev_on(zdev_t *dev);

/** Turn off a device. */
int zio_dev_off(zdev_t *dev);

/** A device operation which does nothing. */
int zio_dev_null(zdev_t *dev);

zdev_t *zio_dev_get(int module, int type, int flags);

void zio_debug(zdev_t *dev);

void zio_dev_error(zdev_t *dev, int err, char *tag);

uint64_t zio_fifo_span(zdev_t *dev);

int zio_write(zdev_t *dev, uint8_t *data, size_t data_len);

int zio_print(zdev_t *dev, int format, char *ret_buf);

void zio_notify(zdev_t *dev);

zdev_t *zio_dev_get_name(zdev_t *dev, const char *name);

void zio_notify_text(zdev_t *dev, char *text);

zdev_t *zio_mod_get(int module, int type);

int zio_write_r(zdev_t *dev, uint8_t *data, size_t data_len, int rep_no);

void zio_digital_mode(int pin, int mode);

void zio_digital_write(int pin, int val);

int zio_digital_read(int pin);

int zio_write16_r(zdev_t *dev, uint8_t *data, size_t data_len, size_t rep_len);

int zio_uart_init(zdev_t *dev, int chan, int baud);

int zio_uart_read(zdev_t *dev, uint8_t *retdata, size_t retdata_len);

int zio_data_ln(zdev_t *dev, uint8_t *data, size_t data_len);

void zio_data_append(zdev_t *dev, uint8_t *data, size_t data_len);

zgeo_t *zio_geo_value(zdev_t *dev);

void zio_mood_incr(zdev_t *dev);

void zio_mood_decr(zdev_t *dev);

int zio_ctl(zdev_t *dev, int reg, void *data);

void zio_init(void);

void zio_term(void);

/** The time (in milliseconds) to wait before accessing a device. */
#define zio_dev_startup_wait(_dev) \
	((_dev)->param.freq_max * 2)


#ifdef HAVE_LIBWIRINGPI

#define ANALOG_READ(_pin) analogRead(ANALOG_PIN_BASE+(_pin))
#define ANALOG_WRITE(_pin, _val) analogWrite(ANALOG_PIN_BASE+(_pin), (_val))
#define ZIO_I2C_INIT(_addr) wiringPiI2CSetup(_addr)
#define ZIO_I2C_READ(_dev) wiringPiI2CRead((_dev)->dev_fd)
#define ZIO_I2C_WRITE(_dev, _val) wiringPiI2CWrite((_dev)->dev_fd, (_val))
#define ZIO_I2C_REG_READ(_dev, _reg) wiringPiI2CReadReg8((_dev)->dev_fd, (_reg))
#define ZIO_I2C_REG_WRITE(_dev, _reg, _val) wiringPiI2CWriteReg8((_dev)->dev_fd, (_reg), (_val))
#define ZIO_I2C_REG16_READ(_dev, _reg) wiringPiI2CReadReg16((_dev)->dev_fd, (_reg))
#define ZIO_I2C_REG16_WRITE(_dev, _reg, _val) wiringPiI2CWriteReg16((_dev)->dev_fd, (_reg), (_val))
#define ANALOG_PIN(_addr) \
	(ANALOG_PIN_BASE + (4 * ((_addr) - DEFAULT_PCF8591_ADDRESS)))
#define PCF8591_INIT(_addr) pcf8591Setup(ANALOG_PIN(_addr), (_addr))

/* softTone.h */
#define ZIO_TONE_INIT(_pin) \
	softToneCreate(_pin)
#define ZIO_TONE_TERM(_pin) \
	softToneWrite((_pin), 0)
#define ZIO_TONE(_pin, _val) \
	softToneWrite((_pin), (_val))


#else /* !WIRINGPI */

#define LOW 0
#define HIGH 1
#define INPUT 0 
#define OUTPUT 1

#define ANALOG_READ(_pin) 0
#define ANALOG_WRITE(_pin, _val)
#define ZIO_I2C_INIT(_addr) 0
#define ZIO_I2C_READ(_dev) 0
#define ZIO_I2C_WRITE(_dev, _val) 0
#define ZIO_I2C_REG_READ(_dev, _reg) 0
#define ZIO_I2C_REG_WRITE(_dev, _reg, _val) 0
#define ZIO_I2C_REG16_READ(_dev, _reg) 0
#define ZIO_I2C_REG16_WRITE(_dev, _reg, _val) 0
#define PCF8591_INIT(_addr) ZERR_INVAL

#endif

#define PIN_MODE(_pin, _mode) \
	(zio_digital_mode((_pin), (_mode)))

#define DIGITAL_WRITE(_pin, _val) \
	 (zio_digital_write((_pin), (_val)))

#define DIGITAL_READ(_pin) \
	(zio_digital_read(_pin))

#define REGISTER_PCF8591_DEVICE() \
	PCF8591_INIT(DEFAULT_PCF8591_ADDRESS)

#ifndef INT_EDGE_FALLING
#define INT_EDGE_FALLING	1
#endif
#ifndef PWM_OUTPUT
#define PWM_OUTPUT		 2
#endif
#ifndef PWM_MODE_MS
#define PWM_MODE_MS		0
#endif

#define ZIO_UART0 0
#define ZIO_UART1 1
#define ZIO_UART2 2

#define ZIO_UART_INIT(_dev, _chan, _baud) \
	(zio_uart_init((_dev), (_chan), (_baud)))
#define ZIO_UART_READ(_dev, _retdata, _retdata_len) \
	(zio_uart_read((_dev), (_retdata), (_retdata_len)))

#endif /* ndef __ZIO_DEV_H__ */


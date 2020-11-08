/* Copyright 2020 Neo Natura */

#include "zio.h"

#define LCD_DISPLAY_LINES 2

#define LCD_DISPLAY_WIDTH 16

#define LCD_BACKLIGHT 0x08

#define LCD_ENABLE 0b00000100

#define LCD_LINE_1 0x80
#define LCD_LINE_2 0xC0

#define LCD_CMD 0
#define LCD_CHR 1

int zio_lcd_twi1602_wait(zdev_t *dev, int bits)
{
	delayMicroseconds(500);
	ZIO_I2C_REG_READ(dev, (bits | LCD_ENABLE));
	delayMicroseconds(500);
	ZIO_I2C_REG_READ(dev, (bits & ~LCD_ENABLE));
	delayMicroseconds(500);
}

int zio_lcd_twi1602_byte(zdev_t *dev, int bits, int mode)
{
	int bits_high;
	int bits_low;

	bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT ;
	bits_low = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT ;

	ZIO_I2C_REG_READ(dev, bits_high);
	zio_lcd_wait(dev, bits_high);

	ZIO_I2C_REG_READ(dev, bits_low);
	zio_lcd_wait(dev, bits_low);
}

int zio_lcd_twi1602_line(zdev_t *dev, int line)
{
	zio_lcd_byte(dev, line, LCD_CMD);
}

void zio_lcd_twi1602_init(zdev_t *dev)
{
	zio_lcd_byte(dev, 0x33, LCD_CMD); /* initialize #1 */
	zio_lcd_byte(dev, 0x32, LCD_CMD); /* initialize #2 */
	zio_lcd_byte(dev, 0x06, LCD_CMD); /* cursor -> direction */
	zio_lcd_byte(dev, 0x0C, LCD_CMD); /* 0x0F Display On, Blink Off */
	zio_lcd_byte(dev, 0x28, LCD_CMD); /* width, height, font */
	zio_lcd_byte(dev, 0x01, LCD_CMD); /* clear display */
}
 
int zio_lcd_twi1602_open(zdev_t *dev)
{
	int err;
	int fd;

	if (is_zio_dev_on(dev))
		return (0);

	fd = ZIO_I2C_INIT(dev->def_pin);
	if (fd < 0)
		return (ZERR_INVAL);

	dev->dev_fd = fd;
	zio_lcd_init(dev);
        zio_dev_on(dev);

	return (0);
}
 
int zio_lcd_twi1602_write(zdev_t *dev, uint8_t *data, size_t data_len)
{
	int i;

	if (!is_zio_dev_on(dev))
		return (ZERR_AGAIN);

	if (*dev->fifo.value) {
		size_t len = strlen(dev->fifo.value);
		zio_lcd_line(dev, LCD_LINE_1);
		for (i = 0; i < LCD_DISPLAY_WIDTH; i++) {
			if (i >= len || data[i] == 0) {
				zio_lcd_byte(dev, ' ', LCD_CHR);		
			} else {
				zio_lcd_byte(dev, dev->fifo.value[i], LCD_CHR);		
			}
		}
	}

	zio_lcd_line(dev, LCD_LINE_2);
	for (i = 0; i < LCD_DISPLAY_WIDTH; i++) {
		if (i >= data_len || data[i] == 0) {
			zio_lcd_byte(dev, ' ', LCD_CHR);
		} else {
			zio_lcd_byte(dev, data[i], LCD_CHR);
		}
	}

	memset(dev->fifo.value, 0, MAX_VALUE_BUFFER_SIZE);
	strncpy(dev->fifo.value, data, MIN(data_len, MAX_VALUE_BUFFER_SIZE-1));

	return (0);
}

int zio_lcd_twi1602_close(zdev_t *dev)
{

	if (!is_zio_dev_on(dev))
		return (0);

	zio_dev_off(dev);

	return (0);
}

zdev_t zio_lcd_twi1602_device =
{
	"lcd16", DEFAULT_LCD_I2C_ADDR, STRATUM_MAX, /* contoller: activity log */
	ZDEV_LOG, DEVF_START | DEVF_OUTPUT, ZMOD_INTERNAL, 
	/* op */
	{ zio_lcd_twi1602_open, NULL, zio_lcd_twi1602_write, NULL, zio_lcd_twi1602_close },
	/* param */
	{ /* freq_min */ 1, /* freq_max */ 60, 0, PIN_NULL }
};


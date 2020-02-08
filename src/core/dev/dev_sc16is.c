/* Copyright 2018 Neo Natura */

#include "zio.h"

#ifdef HAVE_LIBWIRINGPI
#define REG_GET(_fd, _reg) \
	        wiringPiI2CReadReg8(_fd, _reg)
#define REG_SET(_fd, _reg, _val) \
	        wiringPiI2CWriteReg8(_fd, _reg, _val)
#else
#define REG_GET(_fd, _reg) 0
#define REG_SET(_fd, _reg, _val)
#endif

#define SC16IS750_REG_RHR (0x00 << 3)
#define SC16IS750_REG_THR (0X00 << 3)
#define SC16IS750_REG_IER (0X01 << 3)
#define SC16IS750_REG_FCR (0X02 << 3)
#define SC16IS750_REG_IIR (0X02 << 3)
#define SC16IS750_REG_LCR (0X03 << 3)
#define SC16IS750_REG_MCR (0X04 << 3)
#define SC16IS750_REG_LSR (0X05 << 3)
#define SC16IS750_REG_MSR (0X06 << 3)
#define SC16IS750_REG_SPR (0X07 << 3)
#define SC16IS750_REG_TCR (0X06 << 3)
#define SC16IS750_REG_TLR (0X07 << 3)
#define SC16IS750_REG_TXLVL (0X08 << 3)
#define SC16IS750_REG_RXLVL (0X09 << 3)
#define SC16IS750_REG_IODIR (0X0A << 3)
#define SC16IS750_REG_IOSTATE (0X0B << 3)
#define SC16IS750_REG_IOINTENA (0X0C << 3)
#define SC16IS750_REG_IOCONTROL (0X0E << 3)
#define SC16IS750_REG_EFCR (0X0F << 3)
#define SC16IS750_REG_DLL (0x00 << 3)
#define SC16IS750_REG_DLH (0X01 << 3)
#define SC16IS750_REG_EFR (0X02 << 3)
#define SC16IS750_REG_XON1 (0X04 << 3)
#define SC16IS750_REG_XON2 (0X05 << 3)
#define SC16IS750_REG_XOFF1  (0X06 << 3)
#define SC16IS750_REG_XOFF2 (0X07 << 3)

zdev_t *zio_sc16is_dev(int pin)
{
	zdev_t *dev;
	int i2c_addr;

	if (pin >= 80 && pin < 88) {
		i2c_addr = 0x4c;
	} else if (pin >= 80 && pin < 88) {
		i2c_addr = 0x4d;
	} else {
		return (NULL);
	}

	for (dev = zio_device_table; dev; dev = dev->next) {
		if (!(dev->flags & DEVF_I2C))
			continue;
		if (dev->def_pin == i2c_addr)
			return (dev);
	}

	return (NULL);
}

void zio_sc16is_pin_mode(int pin, int mode)
{
	zdev_t *dev;
	uint8_t io;

	dev = zio_sc16is_dev(pin);
	if (!dev)
		return;

	pin = (pin % 8); 

	REG_SET(dev->dev_fd, SC16IS750_REG_IOCONTROL, 0x03);

	io = REG_GET(dev->dev_fd, SC16IS750_REG_IODIR);
	if (mode == OUTPUT) {
		io |= (0x01 << pin);
	} else {
		io &= ~(0x01 << pin);
	}
	REG_SET(dev->dev_fd, SC16IS750_REG_IODIR, io);

}

void zio_sc16is_pin_set(int pin, int state)
{
	zdev_t *dev;
	uint8_t io;

	dev = zio_sc16is_dev(pin);
	if (!dev)
		return;

	pin = (pin % 8);

	io = REG_GET(dev->dev_fd, SC16IS750_REG_IOSTATE);
	if (state != 0) {
		io |= (0x01 << pin);
	} else {
		io &= ~(0x01 << pin);
	}

	REG_SET(dev->dev_fd, SC16IS750_REG_IOSTATE, io);
}

int zio_sc16is_pin_get(int pin)
{
	zdev_t *dev;
	uint8_t io;

	dev = zio_sc16is_dev(pin);
	if (!dev)
		return;

	pin = (pin % 8);

	io = REG_GET(dev->dev_fd, SC16IS750_REG_IOSTATE);
	if (io & (1 << pin))
		return (1);

	return (0);
}

int zio_sc16is_open(zdev_t *dev)
{
	uint8_t reg;
	int fd;

	if (is_zio_dev_on(dev))
		return (0);

	fd = ZIO_I2C_INIT(dev->def_pin);

	/* set baud */
	REG_SET(fd, SC16IS750_REG_LCR, 0x80);
	/* 9600 */
	REG_SET(fd, SC16IS750_REG_DLL, 0x18);
	REG_SET(fd, SC16IS750_REG_DLH, 0x0);
	usleep(1);

	/* turn on exhanced registers */
	REG_SET(fd, SC16IS750_REG_LCR, 0xBF);
	REG_SET(fd, SC16IS750_REG_EFR, 0x10);
	usleep(1);

	REG_SET(fd, SC16IS750_REG_LCR, 0x03); /* 8N1 */
	REG_SET(fd, SC16IS750_REG_FCR, 0x07); /* reset, enable fifo */
	usleep(40); /* wait for crystal to "warm up". */

	/* pin mode: input (as default) */
	REG_SET(fd, SC16IS750_REG_IODIR, 0x0);
	/* enable Rx data ready interrupt */
	REG_SET(fd, SC16IS750_REG_IER, 0x01);

	dev->dev_fd = fd;

	zio_dev_on(dev);

	return (0);
}

int zio_sc16is_read(zdev_t *dev)
{
	int ok;

	if (!is_zio_dev_on(dev))
		return (ZERR_INVAL);

	if (dev->fifo.value_len >= MAX_VALUE_BUFFER_SIZE)
		return (ZERR_OVERFLOW);

	if ((REG_GET(dev->dev_fd, SC16IS750_REG_LSR) & 0x1) == 0)
		return (ZERR_AGAIN); /* nothing to read */

	do {
		dev->fifo.value[dev->fifo.value_len] = 
			REG_GET(dev->dev_fd, SC16IS750_REG_RHR);
		dev->fifo.value_len++;

		if (dev->fifo.value_len >= MAX_VALUE_BUFFER_SIZE)
			break;
	} while (REG_GET(dev->dev_fd, SC16IS750_REG_LSR) & 0x1 != 0);

	return (0);
}

int zio_sc16is_write(zdev_t *dev, uint8_t *data, size_t data_len)
{
	int i;

	if (!is_zio_dev_on(dev))
		return (ZERR_AGAIN);

	for (i = 0; i < data_len; i++)
		REG_SET(dev->dev_fd, SC16IS750_REG_THR, data[i]);

	return (0);
}

int zio_sc16is_close(zdev_t *dev)
{
	zio_dev_off(dev);
}

int zio_sc16is_poll(zdev_t *dev)
{
	int err;

	if (is_zio_dev_on(dev)) {
		err = zio_sc16is_read(dev);
		if (err)
			return (err);
	}

	return (0);
}

int zio_sc16is_ping(zdev_t *dev)
{
	uint8_t reg;
	int ok;

	REG_SET(dev->dev_fd, SC16IS750_REG_SPR, 0x42);
	ok = REG_GET(dev->dev_fd, SC16IS750_REG_SPR);
	if (ok != 0x42)
		return (FALSE);

	return (TRUE);
}

zdev_t zio_sc16is_4c_device =
{
        "sc16is0", 0x4C, 0, /* sc16isXX driver */
        ZDEV_AUDIO, DEVF_START | DEVF_I2C, ZMOD_INTERNAL,
        /* op */
        { zio_sc16is_open, zio_sc16is_read, zio_sc16is_write, NULL, zio_sc16is_close, zio_sc16is_poll },
        /* param */
        { /* freq_min */ 0.25, /* freq_max */ 0.75, 0, PIN_NULL }
};

zdev_t zio_sc16is_4d_device =
{
        "sc16is1", 0x4D, 0, /* sc16isXX driver */
        ZDEV_AUDIO, DEVF_START | DEVF_I2C, ZMOD_INTERNAL,
        /* op */
        { zio_sc16is_open, zio_sc16is_read, zio_sc16is_write, NULL, zio_sc16is_close, zio_sc16is_poll },
        /* param */
        { /* freq_min */ 0.25, /* freq_max */ 0.75, 0, PIN_NULL }
};


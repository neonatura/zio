/* Copyright 2018 Neo Natura */

#include "zio.h"

int zio_i2c_read(zdev_t *dev)
{

	if (dev->param.dev_hid == ZIO_HID_NULL)
		return (ZERR_INVAL);

	if (!is_zio_dev_on(dev)) {
		zio_dev_on(dev);
		dev->dev_fd = ZIO_I2C_INIT(dev->dev_hid);
	}

	return (ZIO_I2C_READ(dev->dev_fd));
}

int zio_i2c_write(zdev_t *dev)
{

	if (dev->param.dev_hid == ZIO_HID_NULL)
		return (ZERR_INVAL);

	if (!is_zio_dev_on(dev)) {
		zio_dev_on(dev);
		dev->dev_fd = ZIO_I2C_INIT(dev->dev_hid);
	}

	return (ZIO_I2C_WRITE(dev->dev_fd));
}

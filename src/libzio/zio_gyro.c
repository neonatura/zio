/* Copyright 2018 Neo Natura */

#include "zio.h"

static int devAddr = MPU6050_DEFAULT_ADDRESS;


/** Power on and prepare for general usage.
 * This will activate the device and take it out of sleep mode (which must be done
 * after start-up). This function also sets both the accelerometer and the gyroscope
 * to their most sensitive settings, namely +/- 2g and +/- 250 degrees/sec, and sets
 * the clock source to use the X Gyro for reference, which is slightly better than
 * the default internal clock source.
 */
void zio_gyro_init(zdev_t *dev)
{
	int fd;

	fd = ZIO_I2C_INIT(dev->dev_fd);
	if (fd == -1) {
		return;
	}

	dev->dev_fd = fd;

	ZIO_I2C_REG_WRITE(dev, MPU6050_RA_SMPLRT_DIV, 0x07); 
	ZIO_I2C_REG_WRITE(dev, MPU6050_RA_PWR_MGMT_1, 0x01);
	ZIO_I2C_REG_WRITE(dev, MPU6050_RA_CONFIG, 0x0);
	ZIO_I2C_REG_WRITE(dev, MPU6050_RA_GYRO_CONFIG, 24);
	ZIO_I2C_REG_WRITE(dev, INT_ENABLE, 0x01);

	zio_dev_on(dev);
}

static int mpu6050_read(zdev_t *dev)
{
	short high_byte,low_byte,value;

	high_byte = ZIO_I2C_REG_READ(dev, addr);
	low_byte = ZIO_I2C_REG_READ(dev, addr+1);
	value = (uint32_t)((high_byte << 8) | low_byte);

	return value;
}

int zio_gyro_read(zdev_t *dev)
{
	double pos[6];

	if (!is_zio_dev_on(dev))
		return (0);

	dev->dvalue = (double *)dev->value;

	/* degress per second */
	dev->dvalue[0] = (double)mpu6050_read(dev, MPU6050_RA_GYRO_XOUT_H) / GYRO_SENSITIVITY_FACTOR;
	dev->dvalue[1] = (double)mpu6050_read(dev, MPU6050_RA_GYRO_YOUT_H) / GYRO_SENSITIVITY_FACTOR;
	dev->dvalue[2] = (double)mpu6050_read(dev, MPU6050_RA_GYRO_ZOUT_H) / GYRO_SENSITIVITY_FACTOR;

	/* accelerometer in "g" (gravity). */
	dev->dvalue[3] = (double)mpu6050_read(dev, MPU6050_RA_ACCEL_XOUT_H) / ACCEL_SENSITIVITY_FACTOR;
	dev->dvalue[4] = (double)mpu6050_read(dev, MPU6050_RA_ACCEL_YOUT_H) / ACCEL_SENSITIVITY_FACTOR;
	dev->dvalue[5] = (double)mpu6050_read(dev, MPU6050_RA_ACCEL_ZOUT_H) / ACCEL_SENSITIVITY_FACTOR;

}

zio_gyro_t *zio_gyro_value(zdev_t *dev)
{
	return ((zio_gyro_t *)dev->value);
}

//MPU6050_DEFAULT_ADDRESS


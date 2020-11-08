/* Copyright 2018 Neo Natura */

#ifndef __ZIO_PIN_H__
#define __ZIO_PIN_H__

/* an "inaccessible" pin */
#define PIN_NULL -1

/* digital pins */
#define PIN_0 0
#define PIN_1 1
#define PIN_2 2
#define PIN_3 3
#define PIN_4 4
#define PIN_5 5
#define PIN_6 6 
#define PIN_7 7
#define PIN_8 8
#define PIN_9 9
#define PIN_15 15
#define PIN_16 16
#define PIN_21 21
#define PIN_22 22
#define PIN_23 23
#define PIN_24 24
#define PIN_25 25
#define PIN_26 26
#define PIN_27 27

/* 
 * SC16IS750 UART I2C 0x4c (A0 HIGH, A1 LOW) 
 *   8 digital I/O pins
 */ 
#define PIN_80 80
#define PIN_81 81
#define PIN_82 82
#define PIN_83 83
#define PIN_84 84
#define PIN_85 85
#define PIN_86 86
#define PIN_87 87

/* 
 * SC16IS750 UART I2C 0x4d (A0/A1 LOW)
 *   8 digital I/O pins
 */
#define PIN_88 88
#define PIN_89 89
#define PIN_90 90
#define PIN_91 91
#define PIN_92 92
#define PIN_93 93
#define PIN_94 94
#define PIN_95 95

/* analog pins */
#define PIN_A0 100 /* RW */
#define PIN_A1 101 /* R */
#define PIN_A2 102 /* R */
#define PIN_A3 103 /* R */
#define PIN_A4 104 /* RW */
#define PIN_A5 105 /* R */
#define PIN_A6 106 /* R */
#define PIN_A7 107 /* R */
#define PIN_A8 108 /* RW */
#define PIN_A9 109 /* R */
#define PIN_A10 110 /* R */
#define PIN_A11 111 /* R */
#define PIN_A12 112 /* RW */
#define PIN_A13 113 /* R */
#define PIN_A14 114 /* R */
#define PIN_A15 115 /* R */
#define PIN_A16 116 /* RW */
#define PIN_A17 117 /* R */
#define PIN_A18 118 /* R */
#define PIN_A19 119 /* R */


#ifdef HAVE_LIBWIRINGPI

/* i2c */
#define PIN_SDA PIN_8
#define PIN_SCL PIN_9

/* pwm modulation */
#define PIN_PWM0 PIN_1
#define PIN_PWM1 PIN_23

/* uart serial */
#define PIN_TXD PIN_15
#define PIN_RXD PIN_16

/* GC-SR501 Motion Sensor */
#define PIN_SR501_0 PIN_0
#define PIN_SR501_1 PIN_2
#define PIN_SR501_2 PIN_3

/* soft-pwm applied to a transistor. */
#define PIN_PWM2 PIN_4
#define PIN_PWM3 PIN_5
#define PIN_PWM4 PIN_6

/* pulled up to trigger relay */
#define PIN_RELAY0 PIN_21
#define PIN_RELAY1 PIN_22

/* [soft] pwm audio output */
#define PIN_SPEAKER PIN_7

#ifdef ZIO_GPSBEE
#define PIN_GPS_TX PIN_TXD
#define PIN_GPS_RX PIN_RXD
#else
#define PIN_GPS_TX PIN_NULL
#define PIN_GPS_RX PIN_NULL
#endif

#ifdef ZIO_LIDAR
#define PIN_LIDAR_TX PIN_TXD
#define PIN_LIDAR_RX PIN_RXD
#else
#define PIN_LIDAR_TX PIN_NULL
#define PIN_LIDAR_RX PIN_NULL
#endif

#ifdef ZIO_GYRO_MPU
#define PIN_GYRO_SDA PIN_SDA
#define PIN_GYRO_SCL PIN_SCL
#define PIN_GYRO_INT PIN_0
#else
#define PIN_GYRO_SDA PIN_NULL
#define PIN_GYRO_SCL PIN_NULL
#define PIN_GYRO_INT PIN_NULL
#endif

/* proprietary DHT11 communication for temp/humidity */
#define PIN_DHT0 PIN_25
#define PIN_DHT0_PWR PIN_24
#define PIN_DHT1 PIN_23

/* dual LEDs for recv/send notification. */
#define PIN_LED0 PIN_26
#define PIN_LED1 PIN_27

#else

#define PIN_PWM0 PIN_NULL
#define PIN_PWM1 PIN_NULL

#define PIN_DHT0 PIN_NULL
#define PIN_DHT1 PIN_NULL
#define PIN_DHT0_PWR PIN_NULL

#define PIN_SPEAKER PIN_NULL

/* GC-SR501 Motion Sensor */
#define PIN_SR501_0 PIN_NULL
#define PIN_SR501_1 PIN_NULL
#define PIN_SR501_2 PIN_NULL

#define PIN_LED0 PIN_NULL
#define PIN_LED1 PIN_NULL

#define PIN_SR501_0 PIN_NULL
#define PIN_SR501_1 PIN_NULL
#define PIN_SR501_2 PIN_NULL

#endif /* HAVE_LIBWIRINGPI */


#define PIN_MOTION PIN_MOTION_0

#define PIN_DHT PIN_DHT0

/* Digital pin output of the SR501 PIR motion sensor. */
#define PIN_MOTION_0 PIN_SR501_0
#define PIN_MOTION_1 PIN_SR501_1
#define PIN_MOTION_2 PIN_SR501_2

#define PIN_LED PIN_LED0
#define PIN_LED_RX PIN_LED0
#define PIN_LED_TX PIN_LED1

#define PIN_COOL_PWR PIN_PWM2
#define PIN_HEAT_PWR PIN_PWM3

#define PIN_LIGHT_PWR PIN_RELAY0
#define PIN_PUMP_PWR PIN_RELAY1


void zio_pin_on(int pin);

void zio_pin_off(int pin);

int zio_analog_init(void);


#endif /* ndef __ZIO_PIN_H__ */


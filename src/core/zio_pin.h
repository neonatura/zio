/* Copyright 2018 Neo Natura */


#define PIN_NULL -1
#define PIN_0 0
#define PIN_1 1
#define PIN_2 2
#define PIN_3 3
#define PIN_4 4
#define PIN_5 5
#define PIN_6 6 
#define PIN_8 8
#define PIN_9 9
#define PIN_15 15
#define PIN_16 16



#ifdef USE_WIRINGPI

/* i2c */
#define PIN_SDA PIN_8
#define PIN_SCL PIN_9

/* pwm modulation */
#define PIN_PWM0 PIN_1

/* uart serial */
#define PIN_TXD PIN_15
#define PIN_RXD PIN_16



#ifdef ZIO_RELAY
/* pulled up to trigger relay in order to turn on a relay. */
#define PIN_RELAY0 PIN_3
#define PIN_RELAY1 PIN_4
#define PIN_RELAY2 PIN_5
#define PIN_RELAY3 PIN_6
#else
#define PIN_RELAY0 PIN_NULL
#define PIN_RELAY1 PIN_NULL
#define PIN_RELAY2 PIN_NULL
#define PIN_RELAY3 PIN_NULL
#endif
#define PIN_FAN_PWR PIN_RELAY0
#define PIN_LAMP_PWR PIN_RELAY1
#define PIN_PUMP_PWR PIN_RELAY2
#define PIN_HEAT_PWR PIN_RELAY3


#ifdef ZIO_AUDIO
/* pwm audio output */
#define PIN_AUDIO_OUT PIN_PWM0
#else
#define PIN_AUDIO_OUT PIN_NULL
#endif

#ifdef ZIO_GPSBEE
#define PIN_GPS_TX PIN_TXD
#define PIN_GPS_RX PIN_RXD
#else
#define PIN_GPS_TX PIN_NULL
#define PIN_GPS_RX PIN_NULL
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

#ifdef ZIO_DHT
/* proprietary DHT11 communication for temp/humidity */
#define PIN_DHT_SIG PIN_2
#else
#define PIN_DHT_SIG PIN_NULL
#endif



void zio_pin_on(int pin);

void zio_pin_off(int pin);

int zio_analog_init(void);


#endif /* USE_WIRINGPI */


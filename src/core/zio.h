/* Copyright 2018 Neo Natura */

#ifndef __ZIO_H__
#define __ZIO_H__

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* autoconf */
#include "config.h"

#ifdef HAVE_LIBWIRINGPI
#include <wiringPi.h>
#include <pcf8591.h>
#endif

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE ~0
#endif

#ifndef MAX
#define MAX(a,b) ((a>b) ? a : b)
#endif
#ifndef MIN
#define MIN(a,b) ((a<b) ? a : b)
#endif

#define MAX_VALUE_BUFFER_SIZE 4096
#define MAX_VALUE_DOUBLE_SIZE 512
#define MAX_VALUE_I32_SIZE 1024
#define MAX_VALUE_I64_SIZE 512

#define ZDEV_NULL 0
#define ZDEV_LOG 1
#define ZDEV_THERM 2 /* temperature */
#define ZDEV_GEO 3 /* lat/lon/ele */
#define ZDEV_AUDIO 4 /* Analog Microphone / Speaker */
#define ZDEV_NET 5 
#define ZDEV_POWER 6 /* Battery Voltage */
#define ZDEV_PHOTO 7 /* Light Intensity */
#define ZDEV_HUMIDITY 8 /* Humidity */
#define ZDEV_MOTION 9 
#define ZDEV_TIME 10


#define ZMOD_NULL 0
#define ZMOD_INTERNAL 1
#define ZMOD_GEO 2

/* device flag to indicate that device is being powered. */
#define DEVF_POWER (1 << 0)
#define DEVF_START (1 << 1)
#define DEVF_INPUT (1 << 2)
#define DEVF_OUTPUT (1 << 3)
#define DEVF_DUMMY (1 << 4)
#define DEVF_MODULE (1 << 5)

#define ZIO_FMT_FAHRENHEIT 1000
#define ZIO_FMT_KELVIN 1001
#define ZIO_FMT_CELCIUS 1002

struct zdev_t;

typedef int (*zio_f)(struct zdev_t *);
typedef int (*zio_reg_f)(struct zdev_t *, int, void *);
typedef int (*zio_data_f)(struct zdev_t *, uint8_t *, size_t);

typedef struct zio_param_t
{

  double freq_min;

  double freq_max;

	/* An (optional) PIN which is capable of turning on a device. */
	int pin_pwr;

	/* working milli-watt rate of power */
	uint32_t rate_pwr;

	/* stratum (permanent degree of trust, lower=better) */
	uint32_t stratum;

	/* A hardware device ID. */
	uint32_t dev_hid;

	/* A ratio at which the cycle frequency is throttled. (0.0 = none) */
	double throttle;

} zio_param_t;

typedef struct zio_fifo_t
{
	size_t value_of;
	size_t value_len;
	uint64_t value_stamp;
  unsigned char value[MAX_VALUE_BUFFER_SIZE];
	uint32_t *ivalue;
	uint64_t *lvalue;
  double *dvalue;
} zio_fifo_t;

typedef struct zio_stat_t
{
  uint32_t read_tot;
  uint32_t write_tot;
  time_t birth_t;
  time_t access_t;

	/* The number of MS between each device poll. */
	uint32_t freq;

	/* The preferred number of MS between each device poll. */
	uint32_t freq_pref;

	/* iteration count */
	uint32_t freq_cycle;

	/* The next time that a poll will take place. */
	uint64_t freq_stamp;
} zio_stat_t;

typedef struct zio_op_t 
{
	/* control and I/O operations for the device. */
	zio_f init; /* initialize the device. */
	zio_f read; /* read data from the device. */
	zio_data_f write; /* write data to the device. */
	zio_reg_f print;
	zio_f term; /* shutdown the device. */
	zio_f timer; /* a continous timer event. */
	zio_reg_f intr; /* an interrupt event. */
} zio_op_t;

typedef struct zdev_t
{
	/* A abreviated name of the device. */
  const char *label;

	/* default data pin */
	int def_pin;

	/* level of accuracy (0best - 255worst) */
	uint32_t stratum;

	uint32_t type;

	/* The state of the device. */
	uint32_t flags;

	uint32_t module;

	zio_op_t op;

	zio_param_t param;

	zio_fifo_t fifo;

	zio_stat_t stat;

	/* The current module that this device is attached to. */
	uint32_t mod_claim;

	/* A posix file descriptor. */
	uint32_t dev_fd;

	struct zdev_t *next;
} zdev_t;

typedef struct zio_gyro_t
{
	double move_x, move_y, move_z;
	double accel_x, accel_y, accel_z;
} zio_gyro_t;


#include "zio_audio.h"
#include "zio_cycle.h"
#include "zio_dev.h"
#include "zio_error.h"
#include "zio_geo.h"
#include "zio_gps.h"
#include "zio_i2c.h"
#include "zio_mod.h"
#include "zio_pin.h"
#include "zio_speaker.h"
#include "zio_time.h"
#include "dev/dev_debug.h"
#include "dev/dev_dht.h"
#include "dev/dev_dummy_temp.h"
#include "dev/dev_itemp.h"
#include "dev/dev_itime.h"
#include "dev/dev_lcd.h"
#include "dev/dev_log.h"
#include "dev/dev_motion.h"
#include "dev/dev_rtc.h"
#include "dev/dev_rtemp.h"
#include "dev/dev_time.h"
#include "dev/dev_therm.h"
#include "dev/dev_thermistor.h"


#endif /* __ZIO_H__ */

/* Copyright 2020 Neo Natura */

#include "ziod.h"

static ztime_t server_start_t;

static int run_state;

entity_t *entity;

void zio_core_term(void)
{
	run_state = RUN_NONE;
}

static void ziod_signal(int sig_no)
{
	zio_core_term();
}

static void ziod_entity_init(char *prog_name, zprocessor_t *zproc)
{
	char name[MAX_ENTITY_NAME_LENGTH];
	char *ptr;
	int err;

	ptr = strrchr(prog_name, '/');
	if (!ptr)
		ptr = strrchr(prog_name, '\\');
	memset(name, 0, sizeof(name));
	strncpy(name, ptr, sizeof(name)-1);
	(void)strtok(name, ".");

	err = htm_entity_init(&entity, name, zproc);
	if (err) {
		zlog_error(err, "htm_entity_init");
	}
}

int ziod_dev_init(zprocessor_t *zproc, zdev_t *dev)
{
	int err;

	err = zio_dev_register(dev);
	if (!err) {
		zproc_io_init(zproc, &dev->conf.dev, &dev->fifo);
	} else {
		/* .. log error .. */
	}

	return (err);
}

void ziod_module_start(zprocessor_t *zproc)
{

  /* diagnostic module */
	ziod_dev_init(zproc, STDOUT_DEVICE()); /* --console */
	ziod_dev_init(zproc, LOG_DEVICE());
	ziod_dev_init(zproc, DIAG_DEVICE()); 

	/* temperature sensor module */
	ziod_dev_init(zproc, ITEMP_DEVICE());
	ziod_dev_init(zproc, THERM_DEVICE());

	/* air quality sensor module */
	ziod_dev_init(zproc, AIR_DEVICE());

	/* lux/light sensor module */
	ziod_dev_init(zproc, LUX_DEVICE());

	/* audio module */
	ziod_dev_init(zproc, AUDIO_DEVICE());

	/* system time module */
	ziod_dev_init(zproc, ITIME_DEVICE());
	ziod_dev_init(zproc, TIME_DEVICE());

}

void ziod_init(const char *prog_name)
{
	zprocessor_t *zproc;
	zdev_t *dev;
	int err;

#ifdef HAVE_LIBWIRINGPI
	wiringPiSetup();
#endif

	/* intialize zpu cores */
	zproc = zpu_init(MAX_ENTITY_BRANES, 0xFFFFFFFF);

	/* initialize devices (libzio) */
//	zio_init();


	signal(SIGINT, ziod_signal);
	signal(SIGFPE, ziod_signal);
	signal(SIGILL, ziod_signal);
	signal(SIGTERM, ziod_signal);

	/* sys periph */
//	REGISTER_LOG_DEVICE();
#ifdef HAVE_LED0_DEVICE
	REGISTER_LED0_DEVICE();
#endif
#ifdef HAVE_LED1_DEVICE
	REGISTER_LED1_DEVICE();
#endif

//	REGISTER_ITIME_DEVICE();
//	REGISTER_TIME_DEVICE(); /* module */

//	REGISTER_DUMMY_TEMP_DEVICE();
//	REGISTER_ITEMP_DEVICE();
#ifdef HAVE_DHT_DEVICE
	/* thermal sensor devices. */
	REGISTER_DHT_DEVICE();
#endif

#ifdef HAVE_GPS_DEVICE
	/* gps device (time) */
	REGISTER_GTIME_DEVICE();
#endif

#ifdef HAVE_RTC_DEVICE
	/* real-time-clock devices. */
	REGISTER_RTEMP_DEVICE();
#endif

#ifdef HAVE_SGP_DEVICE
	/* air periph */
	REGISTER_SGP30_DEVICE();
#endif

#ifdef HAVE_LDR_DEVICE
	/* lux/light sensor devices */
	REGISTER_LDR_DEVICE();
#endif

#ifdef HAVE_SPEAKER_DEVICE
	/* audio devices */
	REGISTER_SPEAKER_DEVICE();
#endif

	/* start device modules */
	ziod_module_start(zproc);

	/* intialize entity (libhtm) */
	ziod_entity_init(prog_name, zproc);

#if 0 /* DEBUG: */
	set_zio_api_func(ZDEV_THERM, ziod_therm_notify);
#endif

	/* diag */
	opt_print();

	zlog_info("application initialized.");

#if 0
	{ /* DEBUG: */
		uint8_t *data;
		size_t data_len;
		int err;

		const uint8_t ch = 'A';

		err = zpu_vaddr_get(zproc, ZPU_ROM_CHARSET_ADDR(ch), &data, &data_len); 
fprintf(stderr, "DEBUG: TEST: %d = zpu_vaddr_get(0x%x[ch %d]): {%x}/<%d bytes>\n", err, ZPU_ROM_CHARSET_ADDR(ch), ch, data, data_len);

		{
			qvar var = (qvar)data;
			size_t len = (size_t)quat_get(var);
			uint8_t *buf;
			int of;
			int y, x;
			int i;

			buf = (uint8_t *)calloc(len, sizeof(uint8_t));
			for (i = 0; i < len; i++) {
				of = (i * 4) + 8;
				var = (qvar)(data + of);
				buf[i] = quat_get32(var); 
			}

			i = 0;
			for (y = 0; y < 8; y++) {
				for (x = 0; x < 6; x++) {
					if (buf[i] != 0x0) {
						printf ("*");
					} else {
						printf (" ");
					}
					i++;
				}
				printf ("\n");
			}
		}
	}
#endif

}

void ziod_term(void)
{
	zprocessor_t *zproc;

	/* libzio */
	zio_term();

	/* libhtm */
	if (entity) zproc = entity->zproc;
	htm_entity_free(&entity);

	zpu_free(&zproc);

	/* libzsys */
	zlog_term();

	/* ziod */
	opt_term();

}

void usage_help(void)
{
  fprintf(stdout,
      "Usage: ziod [OPTIONS]\n"
      "\n"
#ifdef WINDOWS
      "Service Options:\n"
      "--install\tInstall the ziod service.\n"
      "--remove\tRemove the ziod service.\n"
      "\n"
#endif
      "Configuration Options:\n"
      );

  /* configurable runtime options */
  fprintf(stdout, "%s", opt_usage_print());

  fprintf(stdout,
      "\n"
      "Diagnostic Options:\n"
      "\t-nf\n"
      "\t\tRun daemon in foreground (no fork).\n"
      "\n"
      "Visit 'https://neo-natura.com/' for additional information.\n"
      "Report bugs to <support@neo-natura.com>.\n"
      );
}


void usage_version(void)
{
	fprintf(stdout,
			"ziod version %s\n"
			"\n"
			"Copyright 2018 Neo Natural\n"
			"Licensed under the GNU GENERAL PUBLIC LICENSE Version 3\n",
			PACKAGE_VERSION);
}

int zio_core_main(int argc, char *argv[], char *env[])
{

	if (argc >= 2 &&
			(0 == strcmp(argv[1], "-h") ||
			 0 == strcmp(argv[1], "--help"))) {
		usage_help();
		return (0);
	}
	if (argc >= 2 &&
			(0 == strcmp(argv[1], "-v") ||
			 0 == strcmp(argv[1], "--version"))) {
		usage_version();
		return (0);
	}

	server_start_t = ztime();

	/* load configuration options */
	opt_init();
	opt_arg_interp(argc, argv);

	/* initialize entity */
	ziod_init(argv[0]);

	htm_control_state_set(entity, STATE_REST);

	run_state = RUN_IDLE;
	while (run_state) {
		switch (run_state) {
			case RUN_IDLE:
				zio_cycle();
#if 0
				if (htm_control_state_spike(entity)) {
					if (htm_control_state(entity) >= STATE_REST) {
						ziod_therm_cycle(entity);
					}
				}
#endif
				break;
		}
		zio_msleep(1);
	}

	zlog_info("application terminated.");
	ziod_term();

	return (0);
}

int main(int argc, char *argv[])
{

#ifdef WINDOWS
  /* quash stdin/stdout confusion for windows service. */
  (void)open(".tmp-1", O_RDWR | O_CREAT, 0777);
  (void)open(".tmp-2", O_RDWR | O_CREAT, 0777);
  (void)open(".tmp-3", O_RDWR | O_CREAT, 0777);
#endif

	return (zio_core_main(argc, argv, NULL));
}


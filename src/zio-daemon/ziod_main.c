/* Copyright 2020 Neo Natura */

#include "ziod.h"

int run_state;

entity_t *entity;

void zio_core_term(void)
{
	run_state = RUN_NONE;
}

static void ziod_signal(int sig_no)
{
	zio_core_term();
}

void ziod_init(const char *prog_name)
{

	/* register internal devices */
	zio_init();

	zlog_info("application initialized.");

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

	/* time periph */
#ifdef HAVE_GPS_DEVICE
	REGISTER_GTIME_DEVICE();
#endif
//	REGISTER_ITIME_DEVICE();
//	REGISTER_TIME_DEVICE(); /* module */

	/* therm periph */
//	REGISTER_DUMMY_TEMP_DEVICE();
//	REGISTER_ITEMP_DEVICE();
#ifdef HAVE_DHT0_DEVICE
	REGISTER_DHT0_DEVICE();
#endif
#ifdef HAVE_DHT1_DEVICE
	REGISTER_DHT1_DEVICE();
#endif
#ifdef HAVE_DHT2_DEVICE
	REGISTER_DHT2_DEVICE();
#endif
#ifdef HAVE_RTC_DEVICE
	REGISTER_RTEMP_DEVICE();
#endif
//	REGISTER_THERM_DEVICE(); /* module */

	/* air periph */
#ifdef HAVE_SGP_DEVICE
	REGISTER_SGP30_DEVICE();
	REGISTER_AIR_DEVICE(); /* module */
#endif

#ifdef HAVE_LDR_DEVICE
	REGISTER_LDR_DEVICE();
	REGISTER_LUX_DEVICE(); /* module */
#endif

	/* audio */
#ifdef HAVE_SPEAKER_DEVICE
	REGISTER_SPEAKER_DEVICE();
#endif
	REGISTER_AUDIO_DEVICE();

	/* print status to console */
//	REGISTER_STDOUT_DEVICE();

#if 0 /* DEBUG: */
	set_zio_api_func(ZDEV_THERM, ziod_therm_notify);
#endif

	/* diag */
	opt_print();
}

void ziod_term(void)
{
	zio_term();
	opt_term();
}

int zio_core_main(int argc, char *argv[], char *env[])
{
	char name[MAX_ENTITY_NAME_LENGTH];
	char *ptr;
	int err;

	ptr = strrchr(argv[0], '/');
	if (!ptr)
		ptr = strrchr(argv[0], '\\');
	memset(name, 0, sizeof(name));
	strncpy(name, ptr, sizeof(name)-1);
	(void)strtok(name, ".");

  err = htm_entity_init(&entity, name);
	if (err) {
		zlog_error(err, "htm_entity_init");
		return (1);
	}

	/* load configuration options */
	opt_init();

#ifdef HAVE_LIBWIRINGPI
	wiringPiSetup();
#endif

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
	return (zio_core_main(argc, argv, NULL));
}


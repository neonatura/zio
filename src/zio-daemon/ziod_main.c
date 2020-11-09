/* Copyright 2020 Neo Natura */

#include "ziod.h"

int run_state;

entity_t *entity;

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
		zio_error(err, "htm_entity_init", name);
		return (1);
	}

	/* load configuration options */
	opt_init();

#ifdef HAVE_LIBWIRINGPI
	wiringPiSetup();
#endif

	/* sys periph */
	REGISTER_LOG_DEVICE();
#ifdef HAVE_LED0_DEVICE
	REGISTER_LED0_DEVICE();
#endif
#ifdef HAVE_LED1_DEVICE
	REGISTER_LED1_DEVICE();
#endif
	REGISTER_DEBUG_DEVICE();

	/* time periph */
#ifdef HAVE_GPS_DEVICE
	REGISTER_GTIME_DEVICE();
#endif
	REGISTER_ITIME_DEVICE();
	REGISTER_TIME_DEVICE(); /* module */

	/* therm periph */
	REGISTER_DUMMY_TEMP_DEVICE();
	REGISTER_ITEMP_DEVICE();
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
	REGISTER_THERM_DEVICE(); /* module */

	/* air periph */
#ifdef HAVE_SGP_DEVICE
	REGISTER_SGP30_DEVICE();
#endif
	REGISTER_AIR_DEVICE(); /* module */

#ifdef HAVE_LDR_DEVICE
	REGISTER_LDR_DEVICE();
#endif
	REGISTER_LUX_DEVICE(); /* module */

	/* audio */
#ifdef HAVE_SPEAKER_DEVICE
	REGISTER_SPEAKER_DEVICE();
#endif
	REGISTER_AUDIO_DEVICE();

	/* diag */
	opt_print();

	run_state = RUN_IDLE;
	while (run_state) {
		switch (run_state) {
			case RUN_IDLE:
				zio_cycle();
				break;
		}
		zio_msleep(1);
	}

	return (0);
}

void zio_core_term(void)
{
	run_state = RUN_NONE;
}

int main(int argc, char *argv[])
{
	return (zio_core_main(argc, argv, NULL));
}




#include "zio.h"



int main(int argc, char *argv[])
{

	wiringPiSetup();

	/* internal */
	REGISTER_LED_DEVICE();
	REGISTER_LOG_DEVICE();
	REGISTER_DEBUG_DEVICE();

	/* perform task to squak about */
	REGISTER_ITIME_DEVICE();
	REGISTER_TIME_DEVICE();

	REGISTER_SPEAKER_DEVICE();
	REGISTER_AUDIO_DEVICE();

//	zio_audio_intro();




int i;
	for (i = 0; i < 5000; i++) {
		usleep(1000);
		zio_cycle();
	}

//	zio_beep();
//	zio_beep_negative();
//	zio_beep_positive();

//	zio_audio_dtalk("Darb Sri Gun Chai Battle Sword", strlen("Darb Sri Gun Chai Battle Sword"));
//	zio_audio_dtalk("hello world", strlen("hello world"));

	zio_audio_etalk(NULL, "brian", strlen("brian"));

	zio_cycle_loop();

	return (0);
}


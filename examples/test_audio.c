

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

	zio_audio_intro();

#if 0
	zio_beep();
	zio_beep_positive();
#endif

	zio_audio_dtalk("hello world", strlen("hello world"));

	zio_beep_negative();

	zio_cycle_loop();

	return (0);
}


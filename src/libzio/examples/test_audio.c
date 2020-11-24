

#include "zio.h"



int main(int argc, char *argv[])
{
	int i;

	wiringPiSetup();

	/* internal */
	REGISTER_LOG_DEVICE();
	REGISTER_DEBUG_DEVICE();

	/* perform task to squak about */
	REGISTER_ITIME_DEVICE();
	REGISTER_TIME_DEVICE();

	REGISTER_SPEAKER_DEVICE();
	REGISTER_AUDIO_DEVICE();

	/* wait for intro to play (5s) */
	for (i = 0; i < 5000; i++) {
		zio_cycle_step(); 
	}

//	zio_beep();
//	zio_beep_negative();
//	zio_beep_positive();

	zio_audio_dtalk("Darb Sri Gun Chai Battle Sword", strlen("Darb Sri Gun Chai Battle Sword"));

	zio_cycle_loop();

	return (0);
}


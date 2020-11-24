

#include "zio.h"



int main(int argc, char *argv[])
{

	wiringPiSetup();

	/* 128x64 I2C OLED LCD */
	REGISTER_LCD_DEVICE();

	REGISTER_SPEAKER_DEVICE();
	REGISTER_AUDIO_DEVICE();

	/* internal */
	REGISTER_LED_DEVICE();
	REGISTER_LOG_DEVICE();
	REGISTER_DEBUG_DEVICE();
	REGISTER_DUMMY_TEMP_DEVICE();
	REGISTER_ITEMP_DEVICE();
	REGISTER_THERM_DEVICE();
        REGISTER_ITIME_DEVICE();
        REGISTER_TIME_DEVICE();

        /* DS3231 on I2C 0x68 */
        REGISTER_RTC_DEVICE();
        REGISTER_RTEMP_DEVICE();

	/* DHT11 */
	REGISTER_DHT_DEVICE();

	zio_cycle_loop();

	return (0);
}


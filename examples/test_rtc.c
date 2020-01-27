
#include "zio.h"

int main(int argc, char *argv[])
{

	wiringPiSetup();

	/* diagnostic feedback. */
	REGISTER_LED_DEVICE();
	REGISTER_LOG_DEVICE();
	REGISTER_DEBUG_DEVICE();

	/* system clock. */
	REGISTER_ITIME_DEVICE();

	/* ds3231 on i2c 0x68 */
	REGISTER_RTC_DEVICE();

	/* ZIO time module. */
	REGISTER_TIME_DEVICE();

	/* analyze rtc temp */
	REGISTER_ITEMP_DEVICE();
	REGISTER_RTEMP_DEVICE();
        REGISTER_THERM_DEVICE();

	zio_cycle_loop();

	return (0);
}


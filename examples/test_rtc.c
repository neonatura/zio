
#include "zio.h"

int main(int argc, char *argv[])
{

	wiringPiSetup();

	/* diagnostic feedback. */
	REGISTER_DEBUG_DEVICE();

	/* system clock. */
	REGISTER_ITIME_DEVICE();

	/* ds3231 on i2c 0x68 */
	REGISTER_RTC_DEVICE();

	/* ZIO time module. */
	REGISTER_TIME_DEVICE();

	zio_cycle_loop();

	return (0);
}


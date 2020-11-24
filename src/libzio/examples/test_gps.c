
#include "zio.h"

int main(int argc, char *argv[])
{

	wiringPiSetup();

	/* internal */
	REGISTER_LED_DEVICE();
	REGISTER_LOG_DEVICE();
	REGISTER_DEBUG_DEVICE();

	REGISTER_GTIME_DEVICE(); /* gps time */
	REGISTER_ITIME_DEVICE();
	REGISTER_TIME_DEVICE();

	/* gpsbee through xbee through sc16is uart */
	REGISTER_UART1_DEVICE();
	REGISTER_GPS_DEVICE();
	REGISTER_GEO_DEVICE();

	zio_cycle_loop();

	return (0);
}


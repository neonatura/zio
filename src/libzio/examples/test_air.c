
#include "zio.h"

int main(int argc, char *argv[])
{

	wiringPiSetup();

	REGISTER_LED_DEVICE();
	REGISTER_LOG_DEVICE();
	REGISTER_DEBUG_DEVICE();

	REGISTER_DUMMY_TEMP_DEVICE();
	REGISTER_ITEMP_DEVICE();
	REGISTER_DHT_DEVICE();
	REGISTER_THERM_DEVICE();

	REGISTER_SGP30_DEVICE();
	REGISTER_AIR_DEVICE();

	zio_cycle_loop();

	return (0);
}


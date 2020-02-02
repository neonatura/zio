
#include "zio.h"

int main(int argc, char *argv[])
{

	wiringPiSetup();

	/* internal */
	REGISTER_LED_DEVICE();
	REGISTER_LOG_DEVICE();
	REGISTER_DEBUG_DEVICE();

	/* pcf8591 YL-40 */
	REGISTER_PCF8591_DEVICE();
	REGISTER_DUMMY_TEMP_DEVICE();
	REGISTER_ITEMP_DEVICE();
	REGISTER_THERMISTOR_DEVICE();
	REGISTER_THERM_DEVICE(); /* temp module */
	REGISTER_LDR_DEVICE();
	REGISTER_LUX_DEVICE(); /* light module */

	zio_cycle_loop();

	return (0);
}


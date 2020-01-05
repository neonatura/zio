
#include "zio.h"

int main(int argc, char *argv[])
{

	wiringPiSetup();

REGISTER_PCF8591_DEVICE();

  REGISTER_DEBUG_DEVICE();
  REGISTER_DUMMY_TEMP_DEVICE();
  REGISTER_ITEMP_DEVICE();
  REGISTER_THERMISTOR3_DEVICE();
  REGISTER_THERM_DEVICE();

  zio_cycle_loop();

  return (0);
}


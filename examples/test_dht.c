

#include "zio.h"



int main(int argc, char *argv[])
{

wiringPiSetup();

  REGISTER_DEBUG_DEVICE();
  REGISTER_DHT_DEVICE();

  zio_cycle_loop();

  return (0);
}


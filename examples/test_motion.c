

#include "zio.h"



int main(int argc, char *argv[])
{

wiringPiSetup();

  REGISTER_DEBUG_DEVICE();
  REGISTER_MOTION_DEVICE();

  zio_cycle_loop();

  return (0);
}


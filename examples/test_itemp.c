

#include "zio.h"



int main(int argc, char *argv[])
{

  REGISTER_DEBUG_DEVICE();
  REGISTER_ITEMP_DEVICE();

  zio_cycle_loop();

  return (0);
}


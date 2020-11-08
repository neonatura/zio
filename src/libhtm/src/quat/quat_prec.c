/* Copyright 2018 Neo Natura */

#include "quat.h"


int quat_prec(num_t fval)
{
  num_t d;
  uint64_t i;
  int prec;

  i = (uint64_t)fval;
  if (fval == (num_t)i) {
    /* whole number */
    for (prec = 1; prec < (MAX_QUAT_PRECISION-1); prec++) {
      num_t fact = (num_t)powl(QUAT_PRECISION_BASE, prec);
      d = (fval / fact);
      i = (uint64_t)d;
      if ((num_t)i != d)
        break;
    }
    prec--;
  } else {
    /* decimal */
    uint64_t max_fact = (uint64_t)pow(QUAT_PRECISION_BASE, MAX_QUAT_PRECISION);
    uint64_t i_val = (uint64_t)roundl(fval * max_fact);

    for (prec = 1; prec < MAX_QUAT_PRECISION; prec++) {
      uint64_t fact = (uint64_t)pow((double)QUAT_PRECISION_BASE, (double)prec);
      if (0 != (i_val % fact))
        break;
    }
    prec = -1 * (MAX_QUAT_PRECISION - prec + 1);
  }


  return (prec);
}


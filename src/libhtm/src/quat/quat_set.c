/* Copyright 2018 Neo Natura */

#include "quat.h"

/*
static uint32_t test_key[4] = { 0xf7, 0xf7, 0xf7, 0xf7 };
*/

void quat_set(num_t val, int var_type, qvar var)
{
  uint64_t e_val;
  unsigned int flag;
  int prec;

  if (!var)
    return;

  /* sanity */
  val = roundl(val * powl(QUAT_PRECISION_BASE, MAX_QUAT_PRECISION));
  val = val / powl(QUAT_PRECISION_BASE, MAX_QUAT_PRECISION);

  flag = (unsigned int)var_type;
  if (val < 0) flag |= Q_NEG;

  val = fabsl(val);
  prec = quat_prec(val);
  if (prec < 0) {
    val = val * powl(QUAT_PRECISION_BASE, (long double)abs(prec));
  } else {
    val = val / powl(QUAT_PRECISION_BASE, (long double)prec);
  }

  e_val = (uint64_t)val;
  e_val = e_val & QUAT_MASK;
  e_val = htonll(e_val);

  /* fill variable content */
  memcpy(var, &e_val, sizeof(e_val));

  /* fill variable attributes */
  flag += abs(prec + MAX_QUAT_PRECISION);
  quat_var_flag_set(var, flag);
  quat_var_salt_set(var, rand());

	/*
  quat_encode(var, test_key);
	*/

}

void quat_set32(num_t val, int var_type, qvar var)
{
  uint32_t e_val;
  unsigned int flag;
  int prec;

  if (!var)
    return;

  /* sanity */
  val = roundl(val * powl(QUAT_PRECISION_BASE, MAX_QUAT_PRECISION));
  val = val / powl(QUAT_PRECISION_BASE, MAX_QUAT_PRECISION);

  flag = (unsigned int)var_type;
  if (val < 0) flag |= Q_NEG;

  val = fabsl(val);
  prec = quat_prec(val);
  if (prec < 0) {
    val = val * powl(QUAT_PRECISION_BASE, (long double)abs(prec));
  } else {
    val = val / powl(QUAT_PRECISION_BASE, (long double)prec);
  }

  e_val = (uint32_t)val;
  e_val = e_val & QUAT_MASK_32;
  e_val = htonl(e_val);

  /* fill variable content */
  memcpy(var, &e_val, sizeof(e_val));

  /* fill variable attributes */
  flag += abs(prec + MAX_QUAT_PRECISION);
  quat_var_flag_set(var, flag);
  quat_var_salt_set(var, rand());
}

void quat_set64(num_t val, int var_type, qvar var)
{
  uint64_t e_val;
  unsigned int flag;
  int prec;

  if (!var)
    return;

  /* sanity */
  val = roundl(val * powl(QUAT_PRECISION_BASE, MAX_QUAT_PRECISION));
  val = val / powl(QUAT_PRECISION_BASE, MAX_QUAT_PRECISION);

  flag = (unsigned int)var_type;
  if (val < 0) flag |= Q_NEG;

  val = fabsl(val);
  prec = quat_prec(val);
  if (prec < 0) {
    val = val * powl(QUAT_PRECISION_BASE, (long double)abs(prec));
  } else {
    val = val / powl(QUAT_PRECISION_BASE, (long double)prec);
  }

  e_val = (uint64_t)val;
  e_val = e_val & QUAT_MASK;
  e_val = htonl(e_val);

  /* fill variable content */
  memcpy(var, &e_val, sizeof(e_val));

  /* fill variable attributes */
  flag += abs(prec + MAX_QUAT_PRECISION);
  quat_var_flag_set(var, flag);
  quat_var_salt_set(var, rand());
}


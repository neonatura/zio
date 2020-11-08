/* Copyright 2018 Neo Natura */

#include "quat.h"

/*
static uint32_t test_key[4] = { 0xf7, 0xf7, 0xf7, 0xf7 };
*/

num_t quat_get(qvar var)
{
  static const long double neg_fact = -1.0;
  qnum t_var;
  num_t ret_val = 0;
  uint64_t val_bin;
  unsigned int flag;
  unsigned int salt;
  int prec;

  if (!var)
    return (ret_val);

  memcpy(&t_var, var, sizeof(t_var)); 
	/*
  quat_decode(t_var, test_key);
	*/

  flag = quat_var_flag(t_var);
  salt = quat_var_salt(t_var);

  memcpy(&val_bin, t_var, sizeof(val_bin));
  memset(&val_bin, '\000', 2);
  val_bin = ntohll(val_bin);
  ret_val = (num_t)val_bin;
  prec = (flag & 0xf) - MAX_QUAT_PRECISION;
  if (prec < 0) {
    ret_val = ret_val / powl(QUAT_PRECISION_BASE, fabsl((long double)prec));
  } else {
    ret_val = ret_val * powl(QUAT_PRECISION_BASE, (long double)prec);
  }

  if (flag & Q_NEG)
    ret_val *= neg_fact;

  return (ret_val);
}

num_t quat_get32(qvar var)
{
  static const long double neg_fact = -1.0;
  num_t ret_val = 0;
  uint32_t val_bin;
  unsigned int flag;
  unsigned int salt;
  int prec;

  if (!var)
    return (ret_val);

  flag = quat_var_flag(var);
  salt = quat_var_salt(var);

  memcpy(&val_bin, var, sizeof(val_bin));
  memset(&val_bin, '\000', 2);
  val_bin = ntohl(val_bin);
  ret_val = (num_t)val_bin;
  prec = (flag & 0xf) - MAX_QUAT_PRECISION;
  if (prec < 0) {
    ret_val = ret_val / powl(QUAT_PRECISION_BASE, fabsl((long double)prec));
  } else {
    ret_val = ret_val * powl(QUAT_PRECISION_BASE, (long double)prec);
  }

  if (flag & Q_NEG)
    ret_val *= neg_fact;

  return (ret_val);
}


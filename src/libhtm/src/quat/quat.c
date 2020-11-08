/* Copyright 2018 Neo Natura */

#include "quat.h"

size_t quat_var_length(qvar var)
{
  int flag = quat_var_flag(var);

  if (flag & Q_STR) {
    return ((size_t)quat_geti(var));
  } 

  return (0);
}

size_t quat_var_size(qvar var)
{
  int flag = quat_var_flag(var);
  size_t ret_len;

  if (flag & Q_STR) {
    size_t str_len = quat_get(var);

    /* variable length string ( <qnum> + (X * <qchar>) */
    ret_len = sizeof(qnum) + sizeof(qchar) * str_len;
  } else if (flag & Q_NUM) {
    ret_len = sizeof(qnum);
  } else {
    ret_len = sizeof(qchar);
  } 

  return (ret_len);
}

void quat_var_flag_set(qvar var, unsigned int flag)
{
  unsigned char var_flag = (unsigned char)(flag & 0xff);
  unsigned char *raw = (unsigned char *)var;
  raw[0] = var_flag;
}
void quat_var_salt_set(qvar var, unsigned int salt)
{
  unsigned char var_salt = (unsigned char)(salt & 0xff);
  unsigned char *raw = (unsigned char *)var;
  raw[1] = var_salt;
}


unsigned int quat_var_flag(qvar var)
{
  unsigned char *raw = (unsigned char *)var;
  return ((unsigned int)raw[0]);
}

unsigned int quat_var_salt(qvar var)
{
  unsigned char *raw = (unsigned char *)var;
  return ((unsigned int)raw[1]);
}




#ifdef TEST_RUN
/* TEST */
int main(void)
{
  qnum tnum1;
  qnum tnum2;
  qvar tnum3;
  qvar tnum4;
  double tval1 = -111.1111;
  double tval2 = 2220;
  double tval3;
  double tval4;
  tval3 = tval1 + tval2;
  tval4 = tval1 - tval2;
printf ("tval1 %f\n", tval1);
printf ("tval2 %f\n", tval2);
printf ("tval3 %f\n", tval3);
printf ("tval4 %f\n", tval4);

quat_op_init();

  quat_set(tval1, Q_NUM, tnum1);
//printf ("tval1 %llu\n", (unsigned long long)tnum1);
printf ("tnum1 %f\n", quat_getf(tnum1));
  quat_set(tval2, Q_NUM, tnum2);
//printf ("tval2 %llu\n", (unsigned long long)tnum2);
printf ("tnum2 %f\n", quat_getf(tnum2));
  
  tnum3 = quat_op(QOP_SUM, tnum1, tnum2);
//printf ("tval3 %llu\n", (unsigned long long)tnum3);
printf ("tnum3 %f\n", quat_getf(tnum3));

  tnum4 = quat_op(QOP_SUB, tnum1, tnum2);
//printf ("tval4 %llu\n", (unsigned long long)tnum4);
printf ("tnum4 %f\n", quat_getf(tnum4));



{
  qchar text[256];
  quat_sprintf((qvar)text, "hello world\n");
fprintf(stderr, "DEBUG: strlen: %d\n", (int)quat_strlen((qvar)text));
  quat_print((qvar)text);
}
return (0);
}

#endif

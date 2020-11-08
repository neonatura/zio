/* Copyright 2018 Neo Natura */

#include "quat.h"

static void quat_encrypt(uint32_t *v, uint32_t *k)
{   
  uint32_t v0=v[0], v1=v[1], sum=0, i;           /* set up */
  uint32_t delta=0x9e3779b9;                     /* a key schedule constant */
  uint32_t k0=k[0], k1=k[1], k2=k[2], k3=k[3];   /* cache key */
  for (i=0; i < 32; i++) {                       /* basic cycle start */
    sum += delta;
    v0 += ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
    v1 += ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);
  }                                              /* end cycle */
  v[0]=v0; v[1]=v1;
}

void quat_encode(qvar var, qkey key)
{
#if 0
  size_t enc_len;
  int nr;
  int i;

  enc_len = quat_var_size(var);
  if (!enc_len)
    return;

  nr = (enc_len / 8);
  for (i = 0; i < nr; i++) {
    quat_encrypt((uint32_t *)(var + (i*8)), (uint32_t *)key);
  }
#endif
  quat_encrypt((uint32_t *)var, (uint32_t *)key);
}



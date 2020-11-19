/* Copyright 2018 Neo Natura */

#include "quat.h"

static void quat_decrypt(uint32_t *v, uint32_t *k)
{
  uint32_t v0=v[0], v1=v[1], sum=0xC6EF3720, i;  /* set up */
  uint32_t delta=0x9e3779b9;                     /* a key schedule constant */
  uint32_t k0=k[0], k1=k[1], k2=k[2], k3=k[3];   /* cache key */

  for (i=0; i<32; i++) {                         /* basic cycle start */
    v1 -= ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);
    v0 -= ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
    sum -= delta;
  }                                              /* end cycle */
  v[0]=v0; v[1]=v1;
}

void quat_decode(qvar var, qkey key)
{
	uint8_t *raw = (uint8_t *)var;
	uint64_t key_csum;
	uint8_t rawkey[16];
	int size;
	int i;

	size = quat_var_size(var);
	if (size < 16)
		return;

	memset(rawkey, 0, sizeof(key));
	key_csum = (uint64_t)quat_get(key);
	memcpy(rawkey, &key_csum, sizeof(key_csum));
	key_csum = crc64(0, &key_csum, sizeof(key_csum));
	memcpy(rawkey+8, &key_csum, sizeof(key_csum));

	for (i = 8; i < size; i += 8) {
		if ( (size - i) < 8)
			break;
		quat_decrypt((uint32_t *)(raw+i), (uint32_t *)rawkey); 
	}
}



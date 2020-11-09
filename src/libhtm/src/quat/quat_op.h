/* Copyright 2018 Neo Natura */

#ifndef __QUAT_OP_H__
#define __QUAT_OP_H__

/* bit operations */
#define QOP_BIT_ENCODE 10
#define QOP_BIT_DECODE 11
#define QOP_BIT_OR 12 /* merge */
#define QOP_BIT_XOR 13 /* separate */
#define QOP_BIT_AND 14 
#define QOP_BIT_HASH 15
/* math operations */
#define QOP_MATH_SUM 20
#define QOP_MATH_SUB 21
#define QOP_MATH_MULT 22
#define QOP_MATH_DIV 23
#define QOP_MATH_EXP 24
/* mem/str operations */
#define QOP_MEM_CONCAT 30
#define QOP_MEM_TAIL 31
#define QOP_MEM_HEAD 32
#define QOP_MEM_SOUNDEX 33

#define MAX_QOP 64

#define QOP(_func) \
  ((quat_op_f)(_func))

void quat_op_init(void);

void quat_op_impl(int mode, quat_op_f f);

qvar quat_op(int mode, qvar var_a, qvar var_b);

#endif /* ndef __QUAT_OP_H__ */

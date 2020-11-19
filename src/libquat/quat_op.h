/* Copyright 2018 Neo Natura */

#ifndef __QUAT_OP_H__
#define __QUAT_OP_H__

/* bit operations */
#define QOP_BIT_ENCODE 0x10
#define QOP_BIT_DECODE 0x11
#define QOP_BIT_OR 22 /* merge */
#define QOP_BIT_XOR 23 /* separate */
#define QOP_BIT_AND 24 
#define QOP_BIT_HASH 25
#define QOP_MASK_BIT 0x10
/* math operations */
#define QOP_MATH_SUM 0x20
#define QOP_MATH_SUB 0x28
#define QOP_MATH_SLL 0x21
#define QOP_MATH_SLT 0x22
#define QOP_MATH_XOR 0x23
#define QOP_MATH_SRL 0x24
#define QOP_MATH_SRA 0x25
#define QOP_MATH_SUB 0x28
#define QOP_MATH_MUL 0x26
#define QOP_MATH_DIV 0x27
#define QOP_MASK_MATH 0x20
/* mem/str operations */
#define QOP_MEM_CMP 0x30
#define QOP_MEM_CAT 0x31
#define QOP_MEM_TAIL 0x32
#define QOP_MEM_HEAD 0x33
#define QOP_MEM_SOUNDEX 0x34
#define QOP_MASK_MEM 0x30

//#define MAX_QOP 128

#define QOP(_func) \
  ((quat_op_f)(_func))

void quat_op_init(void);

void quat_op_impl(int mode, quat_op_f f);

qvar quat_op(int mode, qvar var_a, qvar var_b);

#endif /* ndef __QUAT_OP_H__ */

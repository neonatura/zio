/* Copyright 2018 Neo Natura */

#include "quat.h"

static quat_table_t _quat_op_table[MAX_QUAT_TABLE];

qvar quat_op(int mode, qvar var_a, qvar var_b)
{
  quat_table_t *table;
  int op_mode;
  int op_cat;

  op_cat = (mode / MAX_QUAT_TABLE);
  if (op_cat < 0 || op_cat >= MAX_QUAT_TABLE)
    return (NULL); /* invalid */

  table = &_quat_op_table[op_cat];
  op_mode = (mode % MAX_QUAT_TABLE);
  if (table->ops[op_mode] == NULL)
    return (NULL); /* not-supported */
  
  return ( (*table->ops[op_mode])(var_a, var_b) );
}

void quat_op_impl(int mode, quat_op_f f)
{
  quat_table_t *table;
  int op_mode;
  int op_cat;

  op_cat = (mode / MAX_QUAT_TABLE);
  op_mode = (mode % MAX_QUAT_TABLE);
  table = &_quat_op_table[op_cat];
  table->ops[op_mode] = f;
}

static qvar quat_op_bit_encode(qvar var_a, qvar var_b)
{
	quat_encode(var_a, var_b);
	return (var_a);
}

static qvar quat_op_bit_decode(qvar var_a, qvar var_b)
{
	quat_decode(var_a, var_b);
	return (var_a);
}

static qvar quat_op_bit_or(qvar var_a, qvar var_b)
{
	const uint64_t a = (uint64_t)quat_get(var_a);
	const uint64_t b = (uint64_t)quat_get(var_b);
	static qnum ret_var;
fprintf(stderr, "DEBUG: quat_op_bit_or: a(%f) b(%f)\n", (double)a, (double)b);
	quat_set((num_t)(a | b), Q_NUM, ret_var);
	return ((qvar)ret_var);
}

static qvar quat_op_bit_xor(qvar var_a, qvar var_b)
{
	static qnum ret_var;
	const uint64_t a = (uint64_t)quat_get(var_a);
	const uint64_t b = (uint64_t)quat_get(var_b);

	quat_set((num_t)(a ^ b), Q_NUM, ret_var);

	return (ret_var);
}

static qvar quat_op_bit_and(qvar var_a, qvar var_b)
{
	static qnum ret_var;
	const uint64_t a = (uint64_t)quat_get(var_a);
	const uint64_t b = (uint64_t)quat_get(var_b);

	quat_set((num_t)(a & b), Q_NUM, ret_var);

	return (ret_var);
}

static qvar quat_op_bit_hash(qvar var_a, qvar var_b)
{
	static qnum ret_var;
	const uint64_t seed = (uint64_t)quat_get(var_a);
	const uint64_t value = (uint64_t)quat_get(var_b);
	uint64_t csum;
	
	csum = crc64(seed, &value, sizeof(value));
	quat_set((num_t)csum, Q_NUM, ret_var);

	return (ret_var);
}

static qvar quat_op_math_sum(qvar var_a, qvar var_b)
{
  static qnum ret_var;
  num_t val_a = quat_get(var_a);
  num_t val_b = quat_get(var_b);
  quat_set(val_a+val_b, Q_NUM, ret_var);
  return ((qvar)ret_var);
}

static qvar quat_op_math_sub(qvar var_a, qvar var_b)
{
  static qnum ret_var;
  num_t val_a = quat_get(var_a);
  num_t val_b = quat_get(var_b);
  quat_set(val_a-val_b, Q_NUM, ret_var);
  return ((qvar)ret_var);
}

void quat_op_init(void)
{

	/* bit */
	quat_op_impl(QOP_BIT_ENCODE, &quat_op_bit_encode);
	quat_op_impl(QOP_BIT_DECODE, &quat_op_bit_decode);
  quat_op_impl(QOP_BIT_OR, &quat_op_bit_or);
  quat_op_impl(QOP_BIT_XOR, &quat_op_bit_xor);
  quat_op_impl(QOP_BIT_AND, &quat_op_bit_and);
  quat_op_impl(QOP_BIT_HASH, &quat_op_bit_hash);

  /* math */
  quat_op_impl(QOP_MATH_SUM, &quat_op_math_sum);
  quat_op_impl(QOP_MATH_SUB, &quat_op_math_sub);

}


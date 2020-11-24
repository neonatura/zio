/* Copyright 2020 Neo Natura */

#include "quat.h"

#define Q_ALLOC 0

qvar quat_alloc(int flag, uint8_t *data, size_t data_len)
{
	qvar ret_var;
	int of;
	int i;

	ret_var = NULL;
	if (flag == Q_NUM) {
		num_t val = *((num_t *)data);
fprintf(stderr, "DEBUG: quat_alloc: Q_NUM: val = %f\n", (double)val);

		ret_var = (qvar)calloc(sizeof(qnum), sizeof(uint8_t));
		if (!ret_var)
			return (NULL);

		quat_set(val, Q_NUM | Q_ALLOC, ret_var); 
fprintf(stderr, "DEBUG: quat_alloc: Q_NUM: retvar = %f\n", (double)quat_get(ret_var));
	} else if (flag == Q_ARRAY) {
		const size_t pagesize = getpagesize(); 
		size_t alloc_len = (8 + (4 * data_len));
		uint8_t *raw;

		alloc_len += (pagesize-1);
		alloc_len /= pagesize;
		alloc_len *= pagesize;
		ret_var = (qvar)calloc(alloc_len, sizeof(uint8_t));
		if (!ret_var)
			return (NULL);

		of = 8;
		raw = (uint8_t *)ret_var;
		for (i = 0; i < data_len; i++) {
			quat_set32((num_t)data[i], Q_CHAR, (qvar)(raw + of));
			of += 4;
		}

		quat_set((num_t)data_len, Q_NUM | Q_ARRAY, ret_var); 
fprintf(stderr, "DEBUG: quat_alloc: Q_ARRAY: (length: %d) (size: %d)\n", data_len, (8 + (4 * data_len)));
	}

	return (ret_var);
}

void quat_free(qvar *var_p)
{
	qvar var;

	if (!var_p)
		return;

	var = *var_p;
	*var_p = NULL;
	if (!var)
		return;

	free(var);
}

int quat_stream(qvar var, uint8_t *retdata, size_t retdata_len)
{
	uint8_t *raw = (uint8_t *)var;
	size_t var_len = (size_t)quat_get(var);
	qvar tvar;
	uint8_t ch;
	int idx;
	int of;

	if (var_len == 0)
		return (0);
	
	for (idx = 0; idx < var_len && idx < retdata_len; idx++) {
		of = (8 + (4 * idx));
		tvar = (qvar)(raw + of);
		ch = (uint8_t)quat_get32(tvar); 
		if (retdata)
			retdata[idx] = ch;
	}

	return (idx);
}


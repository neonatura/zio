/* Copyright 2020 Neo Natura */

#include "quat.h"
#include <stdlib.h>

qvar quat_alloc(int flag, void *data, size_t data_len)
{
	qvar ret_var;

	ret_var = NULL;
	if (flag == Q_NUM) {
		num_t val = *((num_t *)data);
fprintf(stderr, "DEBUG: quat_alloc: Q_NUM: val = %f\n", (double)val);

		ret_var = (qvar)calloc(sizeof(qnum), sizeof(uint8_t));
		quat_set(val, Q_NUM | Q_ALLOC, ret_var); 
fprintf(stderr, "DEBUG: quat_alloc: Q_NUM: retvar = %f\n", (double)quat_get(ret_var));
	} else if (flag == Q_STREAM) {
/* .. */
		//quat_memcpy(ret_var, data, data_len);//quat_set(val, Q_STREAM, data);
	}

	return (ret_var);
}

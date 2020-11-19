/* Copyright 2018 Neo Natura */

#ifndef __QUAT_H__
#define __QUAT_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <arpa/inet.h>

#define QUAT_PRECISION_BASE 10

#define MAX_QUAT_PRECISION 8

#define QUAT_MASK 0xffffffffffff
#define QUAT_MASK_32 0xffff

#define MAX_QUAT_TABLE 100


#ifndef WEOF
#define WEOF (0xffffffffu)
#endif
#define QEOF WEOF

/* the value is negative */
#define Q_NEG (1 << 4)
/* 'char' representation; 32bit if set, 64bit if not */
#define Q_CHAR (1 << 5) 
/* a list of variables */
#define Q_ARRAY (1 << 6)
/* a "named" variable */
#define Q_OBJ (1 << 7)

/* alias */
#define Q_NUM 0
#define Q_STRING Q_ARRAY

#ifndef ntohll
#define ntohll(x) ((1==ntohl(1)) ? (x) : ((uint64_t)ntohl((x) & 0xFFFFFFFF) << 32) | ntohl((x) >> 32))
#endif
#ifndef htonll
#define htonll(x) ((1==htonl(1)) ? (x) : ((uint64_t)htonl((x) & 0xFFFFFFFF) << 32) | htonl((x) >> 32))
#endif

/* generic C number reference. */
typedef long double num_t;

/* generic quat variable reference. */
typedef uint8_t *qvar;

/* generic quat string variable reference. */
typedef uint8_t *qstr;

/* a quat wide-char variable */
typedef uint8_t qchar[8];

/* a quat decimal-precision number variable. */
typedef uint8_t qnum[8];

typedef uint32_t qkey[4];

typedef qvar(*quat_op_f)(qvar, qvar); 


typedef struct quat_table_t {
  quat_op_f ops[MAX_QUAT_TABLE];
} quat_table_t;


#include "quat_op.h"



/* ** quat ** */

size_t quat_var_length(qvar var);

size_t quat_var_size(qvar var);

void quat_var_flag_set(qvar var, unsigned int flag);

void quat_var_salt_set(qvar var, unsigned int salt);

unsigned int quat_var_flag(qvar var);

unsigned int quat_var_salt(qvar var);


/* ** quat_get ** */
num_t quat_get(qvar var);

num_t quat_get32(qvar var);

#define quat_getf(_var) \
  ((double)quat_get(_var))
#define quat_geti(_var) \
  ((int)quat_get(_var))


/* ** quat_set ** */
void quat_set(num_t val, int var_type, qvar var);

void quat_set32(num_t val, int var_type, qvar var);

#define quat_set64(_val, _var_type, _var) \
	(quat_set((_val), (_var_type), (_var)))

/* ** quat_encode ** */
void quat_encode(qvar var, qkey key);

/* ** quat_decode ** */
void quat_decode(qvar var, qkey key);

int quat_prec(num_t fval);

/* ** quat_string ** */
size_t quat_sprintf(qstr str, const char *fmt, ...);
size_t quat_print(qstr str);
size_t quat_strlen(qstr str);

/* ** quat_alloc ** */
qvar quat_alloc(int flag, void *data, size_t data_len);


#endif /* ndef __QUAT_H__ */


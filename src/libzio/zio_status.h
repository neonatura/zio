/* Copyright 2020 Neo Natura */

#ifndef __ZIO_STATUS_H__
#define __ZIO_STATUS_H__

#define STATUS_HEADER_SIZE 1200
#define MAX_STATUS_HEIGHT 10
#define MAX_STATUS_WIDTH 120

#define BLACK 0
#define WHITE 1

typedef struct status_t {
	uint32_t height;
	uint32_t width;
	uint32_t size; 
	uint32_t bits;
	uint64_t l_ops;
	int32_t l_ops_diff;
	unsigned char *header;
} status_t;

status_t *zio_status_init(int width, int height);

void zio_status_render(status_t *stat);

#endif /* ndef __ZIO_STATUS_H__ */


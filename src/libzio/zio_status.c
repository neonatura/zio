/* Copyright 2019 Neo Natura */

#include "zio.h"

extern uint64_t cycle_ops;

status_t *zio_status_init(int width, int height)
{
	status_t *stat;

//	bits = MAX(1, MIN(32, bits));

	stat = (status_t *)calloc(1, sizeof(status_t));
	if (!stat)
		return (NULL);

	stat->header = (unsigned char *)calloc(width * height, 1);//((bits-1)/8)+1);
	if (!stat->header) {
		free(stat);
		return (NULL);
	}

	stat->width = width;
	stat->height = height;
	stat->size = (width * height);
//	stat->bits = MAX(1, MIN(32, bits));

	return (stat);
}

/** Create a image from the runtime status of the program. */
void zio_status_render(status_t *stat)
{
	ztime_t now;
	unsigned int idx;
	int32_t ops_diff;
	int i, j;

	now = zio_time();
  idx = (now/500) % STATUS_HEADER_SIZE;
  ops_diff = (cycle_ops - stat->l_ops);
  stat->l_ops = cycle_ops;

	{ /* black = activity higher than last, white = low-activity */
		if (ops_diff < stat->l_ops_diff)
			stat->header[idx] = BLACK;
		else
			stat->header[idx] = WHITE;
		idx = ((idx+1) % STATUS_HEADER_SIZE);
	}
	stat->l_ops_diff = ops_diff;

	  /* fill-in for future indexes missed. */
  for (i = 0; i < MAX_STATUS_HEIGHT; i++) { /* white padd */
    stat->header[idx] = WHITE;
    idx = ((idx+1) % STATUS_HEADER_SIZE);
  }

	/* left "idle" seperator */
	for (i = 0; i < MAX_STATUS_HEIGHT; i++) { /* black padd */
		stat->header[idx] = BLACK;
		idx = ((idx+1) % STATUS_HEADER_SIZE);
	}
	j = (72 - ops_diff) / 2;
	for (i = 0; i < j; i++) { /* idle */
		stat->header[idx] = BLACK;
		idx = ((idx+1) % STATUS_HEADER_SIZE);
	}

	for (i = 0; i < ops_diff; i++) { /* activity */
		stat->header[idx] = (0 == (idx % 3)) ? BLACK : WHITE;
		idx = ((idx+1) % STATUS_HEADER_SIZE);
	}

	/* right "idle" seperator */
	for (i = 0; i < j; i++) { /* idle */
		stat->header[idx] = BLACK;
		idx = ((idx+1) % STATUS_HEADER_SIZE);
	}
	for (i = 0; i < MAX_STATUS_HEIGHT; i++) { /* black padd */
		stat->header[idx] = BLACK;
		idx = ((idx+1) % STATUS_HEADER_SIZE);
	}

}


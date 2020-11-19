/* Copyright 2020 Neo Natura */

#include "zio.h"

#define ANSI_CLEAR_DISPLAY "\033[2J"
#define ANSI_HOME_LINE "\033[0;0H"
#define ANSI_CLEAR_LINE "\033[K"
#define ANSI_CLEAR_COLOR "\033[0m"

static status_t *status;

void zio_stdout_clearDisplay(void)
{
	printf("%s", ANSI_CLEAR_DISPLAY);
}
void zio_stdout_clearLine(void)
{
	printf("%s", ANSI_CLEAR_LINE);
}

void zio_stdout_homeLine(void)
{
	printf("%s", ANSI_HOME_LINE);
}

void zio_stdout_clearColor(void)
{
	printf ("%s", ANSI_CLEAR_COLOR); 
}

void zio_stdout_init(zdev_t *dev)
{
	status = zio_status_init(MAX_STATUS_WIDTH, MAX_STATUS_HEIGHT);
	fprintf(stderr, "DEBUG: zio_stdout_init: status {%x}\n", status);
	zio_stdout_clearDisplay();
}

/* two lines / 16 pixels height */
void zio_stdout_header(zdev_t *dev)
{
	int l_clr;
	int clr;
	int idx;
	int y;
	int x;
	
	/* border */
	for (idx = 0; idx < 12; idx++) {
		fprintf(stdout, "\033[43m\033[K\n");
	}
	fprintf(stdout, "\033[2;0H\033[0m");
	fflush(stdout);

	zio_status_render(status);

#if 0
	/* draw time */
	{
		char tbuf[32];
		memset(tbuf, 0, sizeof(tbuf));
		strftime(tbuf, sizeof(tbuf)-1, "%H:%M", zio_localtime(now));
		x = 6;
		for (i = 0; i < strlen(tbuf); i++) {
			zio_stdout_drawChar(x, 5, tbuf[i], BLACK, 1);
			x += 6;
		}
	}
#endif

	for (y = 0; y < status->height; y++) {
//		zio_stdout_clearLine();
		fprintf(stdout, "\033[%u;2H", y + 2);
		zio_stdout_clearColor();
		fflush(stdout);

		l_clr = 0;
		for (x = 0; x < status->width; x++) {
			idx = (x * status->height) + y;

			if (status->header[idx]) {
				if (idx != 0 && status->header[idx-1] == 0)
					clr = 45; /* magenta */
				else
					clr = 47; /* white */
			} else {
				clr = 0;
			}

			if (clr == l_clr)
				printf(" ");
			else
				printf("\033[%um ", (unsigned int)clr);
			l_clr = clr;
		}
		printf ("\n");
	}

}
 
int zio_stdout_open(zdev_t *dev)
{
	int err;
	int fd;

	if (is_zio_dev_on(dev))
		return (0);

	zio_stdout_init(dev);

	zio_dev_on(dev);

	return (0);
}
 
int zio_stdout_write(zdev_t *dev, uint8_t *data, size_t data_len)
{
	size_t len;
	size_t of;
	int i;

	if (!is_zio_dev_on(dev))
		return (ERR_AGAIN);

	if (!*data)
		return (0);

	of = dev->fifo.value_len;
	len = MIN(data_len, MAX_VALUE_BUFFER_SIZE - of - 2);
	strcpy(dev->fifo.value + of, data);
	strcat(dev->fifo.value + of, "\n");
	dev->fifo.value_len += (len + 1); 

	return (0);
}

int zio_stdout_close(zdev_t *dev)
{

	if (!is_zio_dev_on(dev))
		return (0);

	memset(dev->fifo.value, 0, MAX_VALUE_BUFFER_SIZE);

	zio_dev_off(dev);

	return (0);
}

int zio_stdout_poll(zdev_t *dev)
{
	uint8_t *raw = dev->fifo.value;
	char buf[32];
	int line;
	int of;

	zio_stdout_homeLine();

	zio_stdout_header(dev);

	printf ("\033[13;0H\033[40;34m\033[K\n");

	line = 0;
	of = stridx(raw, '\n');
	while (of != -1) {
		memset(buf, 0, sizeof(buf));
		strncpy(buf, raw, MIN(sizeof(buf)-1, of));

		zio_stdout_clearLine();
		printf("%s\n", buf);

		raw += (of + 1);
		of = stridx(raw, '\n');

		line++;
		if (line > 6)
			break;
	}

//	zio_stdout_display(dev);

	if (line > 5) {
		/* remove first string. */
		of = stridx(dev->fifo.value, '\n');
		if (of != -1) {
			of++;
			memmove(dev->fifo.value, dev->fifo.value + of, dev->fifo.value_len - of);
			dev->fifo.value_len -= of;

			memset(dev->fifo.value + dev->fifo.value_len, 0, MAX_VALUE_BUFFER_SIZE - dev->fifo.value_len);
		}
	}

	zio_stdout_clearColor();

	return (0);
}

zdev_t zio_stdout_device =
{
	"stdout", PIN_NULL, STRATUM_MAX, /* contoller: activity log */
	ZDEV_DIAG, DEVF_START | DEVF_OUTPUT, ZMOD_INTERNAL, 
	/* op */
	{ zio_stdout_open, NULL, zio_stdout_write, NULL, zio_stdout_close, zio_stdout_poll },
	/* param */
	{ /* freq_min */ 0.5, /* freq_max */ 2, 0.02, PIN_NULL }
};


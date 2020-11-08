/* Copyright 2020 Neo Natura */

#include "zio.h"
#include "dev_lcd_ssd1306_font.h"

#define SSD1306_128_64

#define STATUS_HEADER_SIZE 1200
#define MAX_STATUS_HEIGHT 10
#define MAX_STATUS_WIDTH 120

static unsigned char status_header[STATUS_HEADER_SIZE];
extern uint64_t cycle_ops;

#define true 1
#define false 0

#define rotation 0

#define pgm_read_byte(addr) (*(const unsigned char *)(addr))

int cursor_y = 16;
int cursor_x = 0;

#define BLACK 0
#define WHITE 1
#define INVERSE 2

#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2
#define VCCSTATE SSD1306_SWITCHCAPVCC 

#if defined SSD1306_128_32
        #define WIDTH 128
        #define HEIGHT 32
#endif
#if defined SSD1306_128_64
        #define WIDTH 128
        #define HEIGHT 64
#endif
#if defined SSD1306_96_16
        #define WIDTH 96
        #define HEIGHT 16
#endif
#if defined SSD1306_128_64
        #define SSD1306_LCDWIDTH                  128
        #define SSD1306_LCDHEIGHT                 64
#endif
#if defined SSD1306_128_32
        #define SSD1306_LCDWIDTH                  128
        #define SSD1306_LCDHEIGHT                 32
#endif
#if defined SSD1306_96_16
        #define SSD1306_LCDWIDTH                  96
        #define SSD1306_LCDHEIGHT                 16
#endif

static int buffer[SSD1306_LCDWIDTH * SSD1306_LCDHEIGHT / 8];

#define zio_ssd1306_swap(a, b) { int t = a; a = b; b = t; }

static int stridx(const char *str, char ch)
{
  int i, len;
  if (!str)
    return (-1);
  len =strlen(str);
  for (i = 0; i < len; i++)
    if (str[i] == ch)
      return (i);
  return (-1);
}

// the most basic function, set a single pixel
void zio_ssd1306_drawPixel(int x, int y, unsigned int color)
{
	if ((x < 0) || (x >= WIDTH) || (y < 0) || (y >= HEIGHT))
		return;

	// check rotation, move pixel around if necessary
	switch (rotation) {
	case 1:
		zio_ssd1306_swap(x, y);
		x = WIDTH - x - 1;
		break;
	case 2:
		x = WIDTH - x - 1;
		y = HEIGHT - y - 1;
		break;
	case 3:
		zio_ssd1306_swap(x, y);
		y = HEIGHT - y - 1;
		break;
	}

	// x is which column
	switch (color) {
	case WHITE:
		buffer[x + (y / 8) * SSD1306_LCDWIDTH] |= (1 << (y & 7));
		break;
	case BLACK:
		buffer[x + (y / 8) * SSD1306_LCDWIDTH] &= ~(1 << (y & 7));
		break;
	case INVERSE:
		buffer[x + (y / 8) * SSD1306_LCDWIDTH] ^= (1 << (y & 7));
		break;
	}
}

void zio_ssd1306_command(zdev_t *dev, unsigned int c)
{
	// I2C
	unsigned int control = 0x00;	// Co = 0, D/C = 0
	wiringPiI2CWriteReg8(dev->dev_fd, control, c);
}

void zio_ssd1306_invertDisplay(zdev_t *dev, unsigned int i)
{
	if (i) {
		zio_ssd1306_command(dev, SSD1306_INVERTDISPLAY);
	} else {
		zio_ssd1306_command(dev, SSD1306_NORMALDISPLAY);
	}
}

void zio_ssd1306_display(zdev_t *dev)
{
	zio_ssd1306_command(dev, SSD1306_COLUMNADDR);
	zio_ssd1306_command(dev, 0);	// Column start address (0 = reset)
	zio_ssd1306_command(dev, SSD1306_LCDWIDTH - 1);	// Column end address (127 
	// = reset)

	zio_ssd1306_command(dev, SSD1306_PAGEADDR);
	zio_ssd1306_command(dev, 0);	// Page start address (0 = reset)
#if SSD1306_LCDHEIGHT == 64
	zio_ssd1306_command(dev, 7);	// Page end address
#endif
#if SSD1306_LCDHEIGHT == 32
	zio_ssd1306_command(dev, 3);	// Page end address
#endif
#if SSD1306_LCDHEIGHT == 16
	zio_ssd1306_command(dev, 1);	// Page end address
#endif

	// I2C
	int i;
	for (i = 0; i < (SSD1306_LCDWIDTH * SSD1306_LCDHEIGHT / 8); i++) {
		wiringPiI2CWriteReg8(dev->dev_fd, 0x40, buffer[i]); 
		//This sends byte by byte. 
		//Better to send all buffer without 0x40 first
		//Should be optimized
	}
}

// startscrollright
// Activate a right handed scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// ssd1306_scrollright(0x00, 0x0F)
void zio_ssd1306_startscrollright(zdev_t *dev, unsigned int start, unsigned int stop)
{
	zio_ssd1306_command(dev, SSD1306_RIGHT_HORIZONTAL_SCROLL);
	zio_ssd1306_command(dev, 0X00);
	zio_ssd1306_command(dev, start);
	zio_ssd1306_command(dev, 0X00);
	zio_ssd1306_command(dev, stop);
	zio_ssd1306_command(dev, 0X00);
	zio_ssd1306_command(dev, 0XFF);
	zio_ssd1306_command(dev, SSD1306_ACTIVATE_SCROLL);
}

// startscrollleft
// Activate a right handed scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// ssd1306_scrollright(0x00, 0x0F)
void zio_ssd1306_startscrollleft(zdev_t *dev, unsigned int start, unsigned int stop)
{
	zio_ssd1306_command(dev, SSD1306_LEFT_HORIZONTAL_SCROLL);
	zio_ssd1306_command(dev, 0X00);
	zio_ssd1306_command(dev, start);
	zio_ssd1306_command(dev, 0X00);
	zio_ssd1306_command(dev, stop);
	zio_ssd1306_command(dev, 0X00);
	zio_ssd1306_command(dev, 0XFF);
	zio_ssd1306_command(dev, SSD1306_ACTIVATE_SCROLL);
}

void zio_ssd1306_startscrollup(zdev_t *dev, unsigned int start, unsigned int stop)
{
	zio_ssd1306_command(dev, SSD1306_SET_VERTICAL_SCROLL_AREA);
	zio_ssd1306_command(dev, 0X00);
	zio_ssd1306_command(dev, SSD1306_LCDHEIGHT);
	zio_ssd1306_command(dev, SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL);
	zio_ssd1306_command(dev, 0X00);
	zio_ssd1306_command(dev, start);
	zio_ssd1306_command(dev, 0X00);
	zio_ssd1306_command(dev, stop);
	zio_ssd1306_command(dev, 0X01);
	zio_ssd1306_command(dev, SSD1306_ACTIVATE_SCROLL);
}

// startscrolldiagright
// Activate a diagonal scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// ssd1306_scrollright(0x00, 0x0F)
void zio_ssd1306_startscrolldiagright(zdev_t *dev, unsigned int start, unsigned int stop)
{
	zio_ssd1306_command(dev, SSD1306_SET_VERTICAL_SCROLL_AREA);
	zio_ssd1306_command(dev, 0X00);
	zio_ssd1306_command(dev, SSD1306_LCDHEIGHT);
	zio_ssd1306_command(dev, SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL);
	zio_ssd1306_command(dev, 0X00);
	zio_ssd1306_command(dev, start);
	zio_ssd1306_command(dev, 0X00);
	zio_ssd1306_command(dev, stop);
	zio_ssd1306_command(dev, 0X01);
	zio_ssd1306_command(dev, SSD1306_ACTIVATE_SCROLL);
}

// startscrolldiagleft
// Activate a diagonal scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// ssd1306_scrollright(0x00, 0x0F)
void zio_ssd1306_startscrolldiagleft(zdev_t *dev, unsigned int start, unsigned int stop)
{
	zio_ssd1306_command(dev, SSD1306_SET_VERTICAL_SCROLL_AREA);
	zio_ssd1306_command(dev, 0X00);
	zio_ssd1306_command(dev, SSD1306_LCDHEIGHT);
	zio_ssd1306_command(dev, SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL);
	zio_ssd1306_command(dev, 0X00);
	zio_ssd1306_command(dev, start);
	zio_ssd1306_command(dev, 0X00);
	zio_ssd1306_command(dev, stop);
	zio_ssd1306_command(dev, 0X01);
	zio_ssd1306_command(dev, SSD1306_ACTIVATE_SCROLL);
}

void zio_ssd1306_stopscroll(zdev_t *dev)
{
	zio_ssd1306_command(dev, SSD1306_DEACTIVATE_SCROLL);
}

// Dim the display
// dim = true: display is dimmed
// dim = false: display is normal
void zio_ssd1306_dim(zdev_t *dev, unsigned int dim)
{
	unsigned int contrast;

	if (dim) {
		contrast = 0;	// Dimmed display
	} else {
		if (VCCSTATE == SSD1306_EXTERNALVCC) {
			contrast = 0x9F;
		} else {
			contrast = 0xCF;
		}
	}
	// the range of contrast to too small to be really useful
	// it is useful to dim the display
	zio_ssd1306_command(dev, SSD1306_SETCONTRAST);
	zio_ssd1306_command(dev, contrast);
}

// clear everything
void zio_ssd1306_clearDisplay(void)
{
	memset(buffer, 0,
	       (SSD1306_LCDWIDTH * SSD1306_LCDHEIGHT / 8) * sizeof(int));
//	cursor_y = 0;
	cursor_x = 0;
cursor_y = 16;

}

void ssd1306_drawFastHLineInternal(int x, int y, int w, unsigned int color)
{
	// Do bounds/limit checks
	if (y < 0 || y >= HEIGHT) {
		return;
	}
	// make sure we don't try to draw below 0
	if (x < 0) {
		w += x;
		x = 0;
	}
	// make sure we don't go off the edge of the display
	if ((x + w) > WIDTH) {
		w = (WIDTH - x);
	}
	// if our width is now negative, punt
	if (w <= 0) {
		return;
	}
	// set up the pointer for movement through the buffer
	unsigned int *pBuf = buffer;
	// adjust the buffer pointer for the current row
	pBuf += ((y / 8) * SSD1306_LCDWIDTH);
	// and offset x columns in
	pBuf += x;

	unsigned int mask = 1 << (y & 7);

	switch (color) {
	case WHITE:
		while (w--) {
			*pBuf++ |= mask;
		};
		break;
	case BLACK:
		mask = ~mask;
		while (w--) {
			*pBuf++ &= mask;
		};
		break;
	case INVERSE:
		while (w--) {
			*pBuf++ ^= mask;
		};
		break;
	}
}

void ssd1306_drawFastVLineInternal(int x, int __y, int __h, unsigned int color)
{

	// do nothing if we're off the left or right side of the screen
	if (x < 0 || x >= WIDTH) {
		return;
	}
	// make sure we don't try to draw below 0
	if (__y < 0) {
		// __y is negative, this will subtract enough from __h to account
		// for __y being 0
		__h += __y;
		__y = 0;

	}
	// make sure we don't go past the height of the display
	if ((__y + __h) > HEIGHT) {
		__h = (HEIGHT - __y);
	}
	// if our height is now negative, punt
	if (__h <= 0) {
		return;
	}
	// this display doesn't need ints for coordinates, use local byte
	// registers for faster juggling
	unsigned int y = __y;
	unsigned int h = __h;

	// set up the pointer for fast movement through the buffer
	unsigned int *pBuf = buffer;
	// adjust the buffer pointer for the current row
	pBuf += ((y / 8) * SSD1306_LCDWIDTH);
	// and offset x columns in
	pBuf += x;

	// do the first partial byte, if necessary - this requires some
	// masking
	unsigned int mod = (y & 7);
	if (mod) {
		// mask off the high n bits we want to set
		mod = 8 - mod;

		// note - lookup table results in a nearly 10% performance
		// improvement in fill* functions
		// register unsigned int mask = ~(0xFF >> (mod));
		static unsigned int premask[8] =
		    { 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE };
		unsigned int mask = premask[mod];

		// adjust the mask if we're not going to reach the end of this
		// byte
		if (h < mod) {
			mask &= (0XFF >> (mod - h));
		}

		switch (color) {
		case WHITE:
			*pBuf |= mask;
			break;
		case BLACK:
			*pBuf &= ~mask;
			break;
		case INVERSE:
			*pBuf ^= mask;
			break;
		}

		// fast exit if we're done here!
		if (h < mod) {
			return;
		}

		h -= mod;

		pBuf += SSD1306_LCDWIDTH;
	}
	// write solid bytes while we can - effectively doing 8 rows at a time
	if (h >= 8) {
		if (color == INVERSE) {	// separate copy of the code so we don't
			// impact performance of the black/white
			// write version with an extra comparison
			// per loop
			do {
				*pBuf = ~(*pBuf);

				// adjust the buffer forward 8 rows worth of data
				pBuf += SSD1306_LCDWIDTH;

				// adjust h & y (there's got to be a faster way for me to
				// do this, but this should still help a fair bit for now)
				h -= 8;
			}
			while (h >= 8);
		} else {
			// store a local value to work with
			register unsigned int val = (color == WHITE) ? 255 : 0;

			do {
				// write our value in
				*pBuf = val;

				// adjust the buffer forward 8 rows worth of data
				pBuf += SSD1306_LCDWIDTH;

				// adjust h & y (there's got to be a faster way for me to
				// do this, but this should still help a fair bit for now)
				h -= 8;
			}
			while (h >= 8);
		}
	}
	// now do the final partial byte, if necessary
	if (h) {
		mod = h & 7;
		// this time we want to mask the low bits of the byte, vs the high 
		// bits we did above
		// register unsigned int mask = (1 << mod) - 1;
		// note - lookup table results in a nearly 10% performance
		// improvement in fill* functions
		static unsigned int postmask[8] =
		    { 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F };
		unsigned int mask = postmask[mod];
		switch (color) {
		case WHITE:
			*pBuf |= mask;
			break;
		case BLACK:
			*pBuf &= ~mask;
			break;
		case INVERSE:
			*pBuf ^= mask;
			break;
		}
	}
}

void ssd1306_drawFastHLine(int x, int y, int w, unsigned int color)
{
	unsigned int bSwap = false;
	switch (rotation) {
	case 0:
		// 0 degree rotation, do nothing
		break;
	case 1:
		// 90 degree rotation, swap x & y for rotation, then invert x
		bSwap = true;
		zio_ssd1306_swap(x, y);
		x = WIDTH - x - 1;
		break;
	case 2:
		// 180 degree rotation, invert x and y - then shift y around for
		// height.
		x = WIDTH - x - 1;
		y = HEIGHT - y - 1;
		x -= (w - 1);
		break;
	case 3:
		// 270 degree rotation, swap x & y for rotation, then invert y and 
		// adjust y for w (not to become h)
		bSwap = true;
		zio_ssd1306_swap(x, y);
		y = HEIGHT - y - 1;
		y -= (w - 1);
		break;
	}

	if (bSwap) {
		ssd1306_drawFastVLineInternal(x, y, w, color);
	} else {
		ssd1306_drawFastHLineInternal(x, y, w, color);
	}
}

void ssd1306_drawFastVLine(int x, int y, int h, unsigned int color)
{
	unsigned int bSwap = false;
	switch (rotation) {
	case 0:
		break;
	case 1:
		// 90 degree rotation, swap x & y for rotation, then invert x and
		// adjust x for h (now to become w)
		bSwap = true;
		zio_ssd1306_swap(x, y);
		x = WIDTH - x - 1;
		x -= (h - 1);
		break;
	case 2:
		// 180 degree rotation, invert x and y - then shift y around for
		// height.
		x = WIDTH - x - 1;
		y = HEIGHT - y - 1;
		y -= (h - 1);
		break;
	case 3:
		// 270 degree rotation, swap x & y for rotation, then invert y
		bSwap = true;
		zio_ssd1306_swap(x, y);
		y = HEIGHT - y - 1;
		break;
	}

	if (bSwap) {
		ssd1306_drawFastHLineInternal(x, y, h, color);
	} else {
		ssd1306_drawFastVLineInternal(x, y, h, color);
	}
}

void zio_ssd1306_fillRect(int x, int y, int w, int h, int fillcolor)
{

	// Bounds check
	if ((x >= WIDTH) || (y >= HEIGHT))
		return;

	// Y bounds check
	if (y + h > HEIGHT) {
		h = HEIGHT - y - 1;
	}
	// X bounds check
	if (x + w > WIDTH) {
		w = WIDTH - x - 1;
	}

	switch (rotation) {
	case 1:
		swap_values(x, y);
		x = WIDTH - x - 1;
		break;
	case 2:
		x = WIDTH - x - 1;
		y = HEIGHT - y - 1;
		break;
	case 3:
		swap_values(x, y);
		y = HEIGHT - y - 1;
		break;
	}
	int i;
	for (i = 0; i < h; i++)
		ssd1306_drawFastHLine(x, y + i, w, fillcolor);
}

int textsize = 1;
int wrap = 1;

void ssd1306_setTextSize(int s)
{
	textsize = (s > 0) ? s : 1;
}

void zio_ssd1306_drawChar(int x, int y, unsigned char c, int color, int size)
{

	if ((x >= WIDTH) ||	// Clip right
	    (y >= HEIGHT) ||	// Clip bottom
	    ((x + 6 * size - 1) < 0) ||	// Clip left
	    ((y + 8 * size - 1) < 0))	// Clip top
		return;
	int i;
	int j;
	for (i = 0; i < 6; i++) {
		int line;
		if (i == 5)
			line = 0x0;
		else
			line = pgm_read_byte(font + (c * 5) + i);
		for (j = 0; j < 8; j++) {
			if (line & 0x1) {
				if (size == 1)	// default size
					zio_ssd1306_drawPixel(x + i, y + j, color);
				else {	// big size
					zio_ssd1306_fillRect(x + (i * size),
							 y + (j * size), size,
							 size, color);
				}
			}
			line >>= 1;
		}
	}
}


void zio_ssd1306_write(int c)
{

	if (c == '\n') {
		cursor_y += textsize * 8;
		cursor_x = 0;
	} else if (c == '\r') {
		// skip em
	} else {
		zio_ssd1306_drawChar(cursor_x, cursor_y, c, WHITE, textsize);
		cursor_x += textsize * 6;
		if (wrap && (cursor_x > (WIDTH - textsize * 6))) {
			cursor_y += textsize * 8;
			cursor_x = 0;
		}
	}
}

void zio_ssd1306_drawString(zdev_t *dev, char *str)
{
	int i, end;
	end = strlen(str);
	for (i = 0; i < end; i++)
		zio_ssd1306_write(str[i]);
}

void zio_lcd_ssd1306_init(zdev_t *dev)
{

	// Init sequence
	zio_ssd1306_command(dev, SSD1306_DISPLAYOFF);	// 0xAE
	zio_ssd1306_command(dev, SSD1306_SETDISPLAYCLOCKDIV);	// 0xD5
	zio_ssd1306_command(dev, 0x80);	// the suggested ratio 0x80

	zio_ssd1306_command(dev, SSD1306_SETMULTIPLEX);	// 0xA8
	zio_ssd1306_command(dev, SSD1306_LCDHEIGHT - 1);

	zio_ssd1306_command(dev, SSD1306_SETDISPLAYOFFSET);	// 0xD3
	zio_ssd1306_command(dev, 0x0);	// no offset
	zio_ssd1306_command(dev, SSD1306_SETSTARTLINE | 0x0);	// line #0
	zio_ssd1306_command(dev, SSD1306_CHARGEPUMP);	// 0x8D
	if (VCCSTATE == SSD1306_EXTERNALVCC) {
		zio_ssd1306_command(dev, 0x10);
	} else {
		zio_ssd1306_command(dev, 0x14);
	}
	zio_ssd1306_command(dev, SSD1306_MEMORYMODE);	// 0x20
	zio_ssd1306_command(dev, 0x00);	// 0x0 act like ks0108
	zio_ssd1306_command(dev, SSD1306_SEGREMAP | 0x1);
	zio_ssd1306_command(dev, SSD1306_COMSCANDEC);

#if defined SSD1306_128_32
	zio_ssd1306_command(dev, SSD1306_SETCOMPINS);	// 0xDA
	zio_ssd1306_command(dev, 0x02);
	zio_ssd1306_command(dev, SSD1306_SETCONTRAST);	// 0x81
	zio_ssd1306_command(dev, 0x8F);

#elif defined SSD1306_128_64
	zio_ssd1306_command(dev, SSD1306_SETCOMPINS);	// 0xDA
	zio_ssd1306_command(dev, 0x12);
	zio_ssd1306_command(dev, SSD1306_SETCONTRAST);	// 0x81
	if (VCCSTATE == SSD1306_EXTERNALVCC) {
		zio_ssd1306_command(dev, 0x9F);
	} else {
		zio_ssd1306_command(dev, 0xCF);
	}

#elif defined SSD1306_96_16
	zio_ssd1306_command(dev, SSD1306_SETCOMPINS);	// 0xDA
	zio_ssd1306_command(dev, 0x2);	// ada x12
	zio_ssd1306_command(dev, SSD1306_SETCONTRAST);	// 0x81
	if (VCCSTATE == SSD1306_EXTERNALVCC) {
		zio_ssd1306_command(dev, 0x10);
	} else {
		zio_ssd1306_command(dev, 0xAF);
	}

#endif
	zio_ssd1306_command(dev, SSD1306_SETPRECHARGE);	// 0xd9
	if (VCCSTATE == SSD1306_EXTERNALVCC) {
		zio_ssd1306_command(dev, 0x22);
	} else {
		zio_ssd1306_command(dev, 0xF1);
	}
	zio_ssd1306_command(dev, SSD1306_SETVCOMDETECT);	// 0xDB
	zio_ssd1306_command(dev, 0x40);
	zio_ssd1306_command(dev, SSD1306_DISPLAYALLON_RESUME);	// 0xA4
	zio_ssd1306_command(dev, SSD1306_NORMALDISPLAY);	// 0xA6

	zio_ssd1306_command(dev, SSD1306_DEACTIVATE_SCROLL);

	zio_ssd1306_command(dev, SSD1306_DISPLAYON);	// --turn on oled panel
}



/* two lines / 16 pixels height */
void zio_ssd1306_header(zdev_t *dev)
{
	static uint64_t l_ticks, l_ops;
	static int  l_ops_diff;
uint64_t now;
	unsigned int idx;
	int color;
	int x, y;
	int i;
int j;

	zio_ssd1306_fillRect(0, 0, 128, 16, WHITE);
	zio_ssd1306_fillRect(2, 2, 124, 12, BLACK);

	now = zio_time();
	idx = (now/500) % STATUS_HEADER_SIZE;
	int ops_diff = (cycle_ops - l_ops);
	l_ops = cycle_ops;

	{ /* black = activity higher than last, white = low-activity */
		if (ops_diff < l_ops_diff)
			status_header[idx] = BLACK;
		else
			status_header[idx] = WHITE;
		idx = ((idx+1) % STATUS_HEADER_SIZE);
	}
	l_ops_diff = ops_diff;

	/* fill-in for future indexes missed. */
	for (i = 0; i < MAX_STATUS_HEIGHT; i++) { /* white padd */
		status_header[idx] = WHITE;
		idx = ((idx+1) % STATUS_HEADER_SIZE);
	}

	/* left "idle" seperator */
	for (i = 0; i < MAX_STATUS_HEIGHT; i++) { /* black padd */
		status_header[idx] = BLACK;
		idx = ((idx+1) % STATUS_HEADER_SIZE);
	}
	j = (72 - ops_diff) / 2;
	for (i = 0; i < j; i++) { /* idle */
		status_header[idx] = BLACK;
		idx = ((idx+1) % STATUS_HEADER_SIZE);
	}

	for (i = 0; i < ops_diff; i++) { /* activity */
		status_header[idx] = (0 == (idx % 3)) ? BLACK : WHITE;
		idx = ((idx+1) % STATUS_HEADER_SIZE);
	}

	/* right "idle" seperator */
	for (i = 0; i < j; i++) { /* idle */
		status_header[idx] = BLACK;
		idx = ((idx+1) % STATUS_HEADER_SIZE);
	}
	for (i = 0; i < MAX_STATUS_HEIGHT; i++) { /* black padd */
		status_header[idx] = BLACK;
		idx = ((idx+1) % STATUS_HEADER_SIZE);
	}

	for (i = 0; i < STATUS_HEADER_SIZE; i++) {
		x = (i / MAX_STATUS_HEIGHT) % MAX_STATUS_WIDTH;
		y = (i % MAX_STATUS_HEIGHT);
		zio_ssd1306_drawPixel(x + 4, y + 3, status_header[i]);
	}

	/* draw time */
	{
		char tbuf[32];
		memset(tbuf, 0, sizeof(tbuf));
		strftime(tbuf, sizeof(tbuf)-1, "%H:%M", zio_localtime(now));
		x = 6;
		for (i = 0; i < strlen(tbuf); i++) {
			zio_ssd1306_drawChar(x, 5, tbuf[i], BLACK, 1);
			x += 6;
		}
	}

}
 
int zio_lcd_ssd1306_open(zdev_t *dev)
{
	int err;
	int fd;

	if (is_zio_dev_on(dev))
		return (0);

	fd = ZIO_I2C_INIT(dev->def_pin);
	if (fd < 0)
		return (ZERR_INVAL);

memset(status_header, 1, STATUS_HEADER_SIZE);

	dev->dev_fd = fd;
	zio_lcd_ssd1306_init(dev);

	zio_ssd1306_clearDisplay();
	zio_ssd1306_display(dev);

	zio_dev_on(dev);

	return (0);
}
 
int zio_lcd_ssd1306_write(zdev_t *dev, uint8_t *data, size_t data_len)
{
	size_t len;
	size_t of;
	int i;

	if (!is_zio_dev_on(dev))
		return (ZERR_AGAIN);

	if (!*data)
		return (0);

	of = dev->fifo.value_len;
	len = MIN(data_len, MAX_VALUE_BUFFER_SIZE - of - 2);
	strcpy(dev->fifo.value + of, data);
	strcat(dev->fifo.value + of, "\n");
	dev->fifo.value_len += (len + 1); 

	return (0);
}

int zio_lcd_ssd1306_close(zdev_t *dev)
{

	if (!is_zio_dev_on(dev))
		return (0);

	memset(dev->fifo.value, 0, MAX_VALUE_BUFFER_SIZE);

	zio_ssd1306_clearDisplay();
	zio_ssd1306_display(dev);

	zio_dev_off(dev);

	return (0);
}

int zio_lcd_ssd1306_poll(zdev_t *dev)
{
	uint8_t *raw = dev->fifo.value;
	char buf[32];
	int line;
	int of;

	zio_ssd1306_clearDisplay();

	zio_ssd1306_header(dev);

	line = 0;
	of = stridx(raw, '\n');
	while (of != -1) {
		memset(buf, 0, sizeof(buf));
		strncpy(buf, raw, MIN(sizeof(buf)-1, of));
		zio_ssd1306_drawString(dev, buf);
		zio_ssd1306_write('\n');

		raw += (of + 1);
		of = stridx(raw, '\n');

		line++;
		if (line > 6)
			break;
	}

	zio_ssd1306_display(dev);

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

	return (0);
}

zdev_t zio_lcd_ssd1306_device =
{
	"lcd", DEFAULT_LCD_I2C_ADDR, STRATUM_MAX, /* contoller: activity log */
	ZDEV_LOG, DEVF_START | DEVF_OUTPUT, ZMOD_INTERNAL, 
	/* op */
	{ zio_lcd_ssd1306_open, NULL, zio_lcd_ssd1306_write, NULL, zio_lcd_ssd1306_close, zio_lcd_ssd1306_poll },
	/* param */
	{ /* freq_min */ 0.5, /* freq_max */ 2, 0.02, PIN_NULL }
};


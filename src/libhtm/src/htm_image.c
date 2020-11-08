
/*
 *  Copyright 2020 Brian Burrell
 *
 *  This file is part of libhtm.
 *  (https://github.com/neonatura/zio)
 *
 *  libhtm is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Shionbot is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with libhtm.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "htm.h"

typedef struct pixel_t {
	double red;
	double green;
	double blue;
	double alpha;
	double contrast;
} pixel_t;

void htm_pixel(pixel_t *pix, uint8_t *rgba)
{
	pix->red = ((double)rgba[0] / 256);
	pix->green = ((double)rgba[1] / 256);
	pix->blue = ((double)rgba[2] / 256);
	pix->alpha = ((double)rgba[3] / 256);
	pix->contrast = (double)(rgba[0] + rgba[1] + rgba[2]) * (double)rgba[4] / 195076;
}


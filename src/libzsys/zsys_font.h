
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

#ifndef __HTM_FONT_H__
#define __HTM_FONT_H__

#define FONT_CHAR_HEIGHT 7
#define FONT_CHAR_WIDTH 5

#define pgm_read_byte(addr) (*(const unsigned char *)(addr))

typedef struct image_font8_t {
  uint32_t width;
  uint32_t height;
  uint32_t bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */
  uint8_t pixel[48];
} image_font8_t;

image_t *zfont_init(uint8_t ch, uint32_t color, uint32_t size);

void zfont_free(image_t **image_p);

/**
 * Render a font with a pre-allocated structure.
 */
void zfont_render(uint8_t ch, image_font8_t *image);

#endif /* __HTM_FONT_H__ */

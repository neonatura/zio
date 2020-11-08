
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
 *  libhtm is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with libhtm.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LANG_CHAR_H 
#define LANG_CHAR_H 

typedef struct lang_char {
  uint32_t width;
  uint32_t height;
  uint32_t bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  uint8_t pixel_data[64 * 64 * 4 + 1];
} lang_char;

extern const lang_char *lang_char_table[128];

#define LANG_CHAR(_ch) \
	(lang_char_table[(_ch)])

#endif /* ndef LANG_CHAR_H */





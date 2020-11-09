/*
 * @copyright
 *
 *  Copyright 2020 Neo Natura
 *
 *  This file is part of the zio project.
 *  (https://github.com/neonatura/zio)
 *
 *  The zio project is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The zio project is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with zio project.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  @endcopyright
 */

#ifndef __HTM_BUFF_H__
#define __HTM_BUFF_H__

#define BUFF_PREALLOC (1 << 0)
#define BUFF_FMAP (1 << 1)
#define BUFF_MUTEX (1 << 2)

typedef struct buff_t
{
  unsigned char *data;
  size_t data_of;
  size_t data_max;
  int flags;
#ifdef USE_LIBPTHREAD
  pthread_mutex_t lk;
#endif
} buff_t;

buff_t *buff_map(unsigned char *data, size_t data_len);

buff_t *abuff_map(unsigned char *data, size_t data_len);

unsigned char *buff_unmap(buff_t *buf);

buff_t *buff_init(void);

int buff_grow(buff_t *buf, size_t data_len);

void buff_catstr(buff_t *buf, char *data);

void buff_cat(buff_t *buf, void *data, size_t data_len);

void buff_memcpy(buff_t *buf, void *data, size_t data_len);

size_t buff_idx(buff_t *buf, unsigned char ch);

size_t buff_size(buff_t *buf);

unsigned char *buff_data(buff_t *buf);

void buff_clear(buff_t *buf);

void buff_trim(buff_t *buf, size_t len);

void buff_truncate(buff_t *buf, size_t len);

void buff_dealloc(buff_t *buf);

void buff_free(buff_t **buf_p);

void buff_append(buff_t *from_buff, buff_t *to_buff);

buff_t *buff_clone(buff_t *buff);

int buff_sprintf(buff_t *buff, char *fmt, ...);

int buff_cmp(buff_t *buff, buff_t *cmp_buff);

void buff_padd(buff_t *buff, size_t len);

#endif /* ndef __HTM_BUFF_H__ */

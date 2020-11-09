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

#include "htm.h"

#define MAX_BUFF_SIZE 0xFFFFFFFFFFFF

static void buff_lock_init(buff_t *buff)
{
#ifdef USE_LIBPTHREAD
	pthread_mutexattr_t attr;

	if (!buff)
		return;

	memset(&attr, 0, sizeof(attr));
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&buff->lk, &attr);

  buff->flags |= BUFF_MUTEX;
#endif
}

buff_t *buff_map(unsigned char *data, size_t data_len)
{
  buff_t *buf;

  buf = (buff_t *)calloc(1, sizeof(buff_t));
	if (!buf)
		return (NULL);

  buf->data = data;
  buf->data_of = data_len;
  buf->data_max = data_len;
  buf->flags |= BUFF_PREALLOC;
	buff_lock_init(buf);

  return (buf);
}

buff_t *abuff_map(unsigned char *data, size_t data_len)
{
  static buff_t ret_buf;

  memset(&ret_buf, 0, sizeof(ret_buf));
  ret_buf.data = data;
  ret_buf.data_of = data_len;
  ret_buf.data_max = data_len;

  return (&ret_buf);
}

unsigned char *buff_unmap(buff_t *buf)
{
  unsigned char *data = buff_data(buf);
  free(buf);
  return (data);
}

static int __buff_growmap(buff_t *buf, size_t data_len)
{
  struct stat st;
  size_t block_size;
  size_t of;
  char *data;
  void *map_data;
  void *map_newdata;
  size_t map_len;
  size_t map_newlen;
  ssize_t w_len;
  int err;

  map_data = NULL;
  map_len = 0;
  if (buf->data) {
    map_data = buf->data;
    //map_len = buf->data_of;
    map_len = buf->data_max;
  }

  if (map_len >= data_len)
    return (0); /* bufmap exceeds allocation requested */

  buf->data = NULL;
  buf->data_max = 0;

  block_size = sysconf(_SC_PAGE_SIZE);

	map_newlen = ((data_len / block_size) + 1) * block_size;
	map_newdata = mmap(NULL, map_newlen, PROT_READ | PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, 0, 0); 
  if (map_newdata == MAP_FAILED) {
    return (SHERR_NOBUFS);
	}

  if (map_data) {
		/* copy original content. */
		memcpy(map_newdata, map_data, map_len);

    munmap(map_data, map_len); /* ignore EINVAL return */
  }

  buf->data = map_newdata;
  buf->data_max = map_newlen;
  buf->flags |= BUFF_FMAP;

  return (0);
}

int buff_growmap(buff_t *buf, size_t data_len)
{
	int ret_val;

	if (!buf)
		return (SHERR_INVAL);

	buff_lock(buf);
	ret_val = __buff_growmap(buf, data_len);
	buff_unlock(buf);

	return (ret_val);
}

buff_t *buff_init(void)
{
  buff_t *buf;

  buf = (buff_t *)calloc(1, sizeof(buff_t));
	if (!buf)
		return (NULL);

	buff_lock_init(buf);
  return (buf);
}

static int __buff_grow(buff_t *buf, size_t data_len)
{
  size_t orig_len;
  int block_len;

  block_len = ((data_len + 1) / 16384) + 1;
  if ((block_len * 16384) <= buf->data_max)
    return (0); /* already allocated */

  if (buf->flags & BUFF_FMAP)
    return (SHERR_OPNOTSUPP);

  orig_len = buf->data_max;
  buf->data_max = block_len * 16384;
  if (!buf->data) {
    buf->data = (char *)calloc(buf->data_max, sizeof(char));
  } else {// if (buf->data_of + data_len >= buf->data_max) {
    unsigned char *orig_data = (unsigned char *)buf->data;
    buf->data = (char *)realloc(buf->data, buf->data_max);
    if (!buf->data) { /* realloc not guaranteed */
      char *data = (char *)calloc(buf->data_max, sizeof(char));
      if (data) {
        memcpy(data, orig_data, orig_len);
        free(orig_data);
        buf->data = data;
      }
    }
  } 

  if (!buf->data) {
    buf->data_max = buf->data_of = 0;
    return (SHERR_NOBUFS);
  }

  return (0);
}

int buff_grow(buff_t *buf, size_t data_len)
{
	int ret_val;

	if (!buf)
		return (SHERR_INVAL);

	buff_lock(buf);
	ret_val = __buff_grow(buf, data_len);
	buff_unlock(buf);

	return (ret_val);
}

void buff_catstr(buff_t *buf, char *data)
{

	if (!data)
		return;

  buff_cat(buf, (unsigned char *)data, strlen(data));
}

static void __buff_cat(buff_t *buf, void *data, size_t data_len)
{
  int err;

  err = buff_grow(buf, buf->data_of + data_len + 1);
  if (err) {// && err != SHERR_OPNOTSUPP) {
    return; 
  }

  memcpy(buf->data + buf->data_of, data, data_len);
  buf->data_of += data_len;
}

void buff_cat(buff_t *buf, void *data, size_t data_len)
{

	if (!buf)
		return;

	buff_lock(buf);
	__buff_cat(buf, data, data_len);
	buff_unlock(buf);
}

static void __buff_memcpy(buff_t *buf, void *data, size_t data_len)
{
  unsigned char *buf_data = buff_data(buf); 
  size_t buf_len = MIN(data_len, buff_size(buf));
  memcpy(data, buf_data, buf_len);
}
void buff_memcpy(buff_t *buf, void *data, size_t data_len)
{

	if (!buf)
		return;

	buff_lock(buf);
	__buff_memcpy(buf, data, data_len);
	buff_unlock(buf);
}

size_t buff_idx(buff_t *buf, unsigned char ch)
{
  int i;

	if (!buf)
		return (-1);

  for (i = 0; i < buf->data_of; i++) {
    if (buf->data[i] == ch)
      return (i);   
  }

  return (-1);
}

size_t buff_size(buff_t *buf)
{

  if (!buf)
    return (0);

  return (buf->data_of);
}

unsigned char *buff_data(buff_t *buf)
{

  if (!buf)
    return (NULL);

  return (buf->data);
}

/**
 * May consider a hook here to trim contents of maximum buffer size or swap in/out of a cache pool.
 */
void buff_clear(buff_t *buf)
{

  if (!buf)
    return;

  buff_trim(buf, buf->data_of);
}

static void __buff_trim(buff_t *buf, size_t len)
{
  size_t nlen;

  if (!buf->data)
    return;

	if (len > MAX_BUFF_SIZE) {
		return;
	}

	if (len > buf->data_of)
		len = buf->data_of;
  if (len == 0)
    return;

  if (buf->data_of == len) {
    memset(buf->data, 0, len);
    buf->data_of = 0;
  } else {
		nlen = MAX(0, (ssize_t)buf->data_of - (ssize_t)len);
		memmove(buf->data, buf->data + len, nlen);
		buf->data_of = nlen;
		memset(buf->data + nlen, 0, buf->data_max - nlen);
	}

}
void buff_trim(buff_t *buf, size_t len)
{

	if (!buf)
		return;

	buff_lock(buf);
	__buff_trim(buf, len);
	buff_unlock(buf);
}

void buff_truncate(buff_t *buf, size_t len)
{
  size_t nlen;

  if (!buf || !buf->data)
    return;

  len = MIN(len, buf->data_of);
  if (len == 0)
    return;

  if (buf->data_of == len) {
    return;
  }

  nlen = buf->data_of - len;
  memset(buf->data + len, 0, nlen);
  buf->data_of = len;

}

void buff_dealloc(buff_t *buf)
{
  if (buf->flags & BUFF_FMAP) {
    munmap(buf->data, buf->data_max);
  } else if (buf->data) {
    free(buf->data);
  }
  free(buf);
}

static void buff_lock_free(buff_t *buff)
{
#ifdef USE_LIBPTHREAD
	if (!buff)
		return;

  if (!(buff->flags & BUFF_MUTEX))
		return;

	pthread_mutex_destroy(&buff->lk);
  buff->flags &= ~BUFF_MUTEX;
#endif
}

void buff_free(buff_t **buf_p)
{
  buff_t *buf = *buf_p;

  if (!buf)
    return;

#ifdef USE_LIBPHREAD
	/* try to wait for other threads to finish (not atomic). */
	pthread_yield();
	buff_lock(buf);
	buff_unlock(buf);
#endif

	/* de-alloc pthread mutex */
	buff_lock_free(buf);

  if (buf->flags & BUFF_PREALLOC)
    buf->data = NULL; /* prevent free on pre-alloc'd data */
  buff_dealloc(buf);
  *buf_p = NULL;
}

void buff_append(buff_t *from_buff, buff_t *to_buff)
{
  if (!from_buff || !to_buff)
    return;
  buff_cat(to_buff, buff_data(from_buff), buff_size(from_buff));
}

buff_t *buff_clone(buff_t *buff)
{
  buff_t *ret_buf;

  ret_buf = buff_init();
  buff_append(buff, ret_buf);

  return (ret_buf);
}

int buff_sprintf(buff_t *buff, char *fmt, ...)
{
  va_list ap;
  char *str = NULL;
  char tfmt[256];
  int ret_len;
  int is_escape;
  int len;
  int i, j;

  buff_clear(buff);

  if (!fmt)
    return (0);

	buff_lock(buff);

  ret_len = 0;
  is_escape = FALSE;
  va_start(ap, fmt);
  for(i=0;i<strlen(fmt);i++) {
    if (!is_escape) {
      if (fmt[i] == '%') {
        strcpy(tfmt, "%");
        is_escape = TRUE;
        continue;
      }
      buff_cat(buff, fmt + i, 1);
      ret_len++;
      continue;
    } 

    sprintf(tfmt+strlen(tfmt), "%c", fmt[i]);
    switch(fmt[i]) {
      case '%':
        buff_catstr(buff, "%");
        is_escape = FALSE;
        ret_len++;
        break;

      case 's':
        str = (char *)va_arg(ap, char *);
        len = strlen(str) * 2;
        buff_growmap(buff, ret_len + len);
        len = vsnprintf(buff->data + buff->data_of, len, tfmt, ap);
        ret_len += len;
        buff->data_of += len;
        is_escape = FALSE;
        break;

      case 'd':
      case 'u':
      case 'x':
      case 'c':
        buff_growmap(buff, ret_len + 64);
        len = vsnprintf(buff->data + buff->data_of, 64, tfmt, ap);
        ret_len += len;
        buff->data_of += len;
        is_escape = FALSE;
        break;
    }
  }
  va_end(ap);

	buff_unlock(buff);

  return (ret_len);
}
 
int buff_cmp(buff_t *buff, buff_t *cmp_buff)
{
  if (buff_size(buff) != buff_size(cmp_buff))
    return (FALSE);
  if (0 != memcmp(buff_data(buff), buff_data(cmp_buff), buff_size(buff)))
    return (FALSE);
  return (TRUE);
}

/**
 * Increase the data size of a buffer [with zero ('\000') byte data] from it's current size until it reaches <len> bytes.
 */
void buff_padd(buff_t *buff, size_t len)
{
  static const char null[8];
  size_t of;

  for (of = buff_size(buff); of < len; of++) {
    buff_cat(buff, null, 1);
  }

}

void buff_lock(buff_t *buff)
{
#ifdef USE_LIBPTHREAD
	if (!buff)
		return;

	if (!(buff->flags & BUFF_MUTEX))
		return;

	(void)pthread_mutex_lock(&buff->lk);
#endif
}

void buff_unlock(buff_t *buff)
{
#ifdef USE_LIBPTHREAD
	if (!buff)
		return;

	if (!(buff->flags & BUFF_MUTEX))
		return;

	(void)pthread_mutex_unlock(&buff->lk);
#endif
}

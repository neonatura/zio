
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

#include "zsys.h"

static int _zlog_fd;
static int _zlog_debug = 1;

void zlog(int level, int code, char *text)
{
	char tbuf[32];
	time_t now;

	if (!_zlog_fd) {
		const char *log_dir = zsys_log_path();
		char path[PATH_MAX+1];
		int fd;

		(void)mkdir(log_dir, 0777);
		sprintf(path, "%s/zio.log", log_dir);
		fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0666);
		if (fd == -1) return;
		_zlog_fd = fd;
	}

	now = time(NULL);
	memset(tbuf, 0, sizeof(tbuf));
	strftime(tbuf, sizeof(tbuf)-1, "[%H:%M:%S] ", localtime(&now));
	write(_zlog_fd, tbuf, strlen(tbuf));
	if (code && code != ZERR_INFO) {
		char *err_str = zstrerror(code);
		if (err_str && *err_str) {
			write(_zlog_fd, err_str, strlen(err_str));
			write(_zlog_fd, ": ", 2);
		}
	}
	write(_zlog_fd, text, strlen(text));
	write(_zlog_fd, "\n", 1);
}

void f_zsys_log(int err_code, const char *tag, const char *text, const char *src_fname, long src_line)
{
  static buff_t *buff;
  char fname[PATH_MAX+1];
  char origin[256];
  char *date_str;
  char buf[256];
  size_t len;

	if (!err_code && !_zlog_debug)
    return;

  if (!buff)
    buff = buff_init();

  if (tag) {
    buff_catstr(buff, (char *)tag);
    buff_catstr(buff, ": ");
  }
  if (text) {
    len = strlen(text);
    if (*text && text[strlen(text)-1] == '\n')
      len--;
    buff_cat(buff, text, len);
  }
  if (src_fname && src_line) {
    char *ptr = strrchr(src_fname, '/');
    if (!ptr)
      strncpy(fname, src_fname, sizeof(fname)-1);
    else
      strncpy(fname, ptr+1, sizeof(fname)-1);

    sprintf(origin, " (%s:%ld)", fname, src_line);
    buff_catstr(buff, origin);
  }

  if (err_code && err_code != ZERR_INFO) {
		zlog(ZLOG_ERROR, err_code, buff_data(buff));
  } else {
    zlog(ZLOG_INFO, err_code, buff_data(buff));
  }

  buff_clear(buff);
}

void zlog_term(void)
{
	if (_zlog_fd != 0) {
		close(_zlog_fd);
		_zlog_fd = 0;
	}
}

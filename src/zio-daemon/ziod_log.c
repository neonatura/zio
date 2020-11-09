
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

#include "ziod.h"

void f_zio_log(int err_code, const char *tag, const char *text, const char *src_fname, long src_line)
{
  static buff_t *buff;
  char fname[PATH_MAX+1];
  char origin[256];
  char *date_str;
  char buf[256];
  size_t len;

  if (!err_code && !opt_num(OPT_DEBUG)) {
    return;
  }

  if (!buff)
    buff = buff_init();

  if (tag) {
    buff_catstr(buff, (char *)tag);
    buff_catstr(buff, ": ");
  }
	if (err_code && err_code != ZERR_INFO) {
		char *err_str = zio_strerror(err_code);
		//buff_catstr(buff, "error: ");
		buff_catstr(buff, err_str);
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
    zlog(ZLOG_ERROR, 0, buff_data(buff));
  } else {
    zlog(ZLOG_INFO, 0, buff_data(buff));
  }

  buff_clear(buff);
}

#if 0
void timing_init(char *tag, ztime_t *stamp_p)
{
  
	/* mark the time when we started timing. */
  *stamp_p = shtime();

}

void timing_term(int ifaceIndex, char *tag, ztime_t *stamp_p)
{
  ztime_t stamp = *stamp_p;
  double diff = shtime_diff(stamp, shtime());
  char buf[256];

  if (diff >= 0.5) { /* 1/2 a second */
		memset(buf, 0, sizeof(buf));
		snprintf(buf, sizeof(buf)-1,
				"TIMING[%s]: total %-2.2f seconds.", tag, diff);
		zio_log(buf);
  }

}
#endif

static FILE *net_fl;
void f_zio_log_net(const char *iface, const char *addr, const char *tag, size_t size, const char *src_fname, long src_line)
{
	static buff_t *buff;
	static time_t last_t;
	char fname[PATH_MAX+1];
	char origin[256];
	char *date_str;
	char buf[256];
	size_t len;
	time_t now;

	if (!buff)
		buff = buff_init();

	now = time(NULL);
	strftime(buf, sizeof(buf)-1, "[%D %T] ", localtime(&now));
	buff_catstr(buff, buf);
	buff_catstr(buff, iface);
	buff_catstr(buff, " ");

	if (addr) {
		buff_catstr(buff, (char *)addr);
		buff_catstr(buff, ": ");
	}
	if (tag) {
		buff_catstr(buff, (char *)tag);
		buff_catstr(buff, " ");
	}
	sprintf(buf, "%ub", (unsigned int)size);
	buff_catstr(buff, (char *)buf);

#if 0
	if (src_fname && src_line) {
		char *ptr = strrchr(src_fname, '/');
		if (!ptr)
			strncpy(fname, src_fname, sizeof(fname)-1);
		else
			strncpy(fname, ptr+1, sizeof(fname)-1);

		sprintf(origin, "(%s:%ld)", fname, src_line);
		buff_catstr(buff, origin);
	}
#endif

	if (!net_fl) {
		char path[PATH_MAX+1];
		sprintf(path, "%snet.log", zio_log_path());
		net_fl = fopen(path, "ab");
	}
	if (net_fl)
		fprintf(net_fl, "%s\n", buff_data(buff));

	if (last_t != now) {
		fflush(net_fl);
		last_t = now;
	}

	buff_clear(buff);
}

void zio_log_net_free(void)
{
	if (net_fl) {
		fclose(net_fl);
	}
	net_fl = NULL;
}

static char _log_path[PATH_MAX+1];
const char *zio_log_path(void)
{
	const char *tag = "zio";

  if (!*_log_path) {
    if (!tag)
      tag = "share";

    /* default log directory */
#ifdef WINDOWS
    sprintf(_log_path, "%s\\%s\\", getenv("ProgramData"), tag);
#else
    sprintf(_log_path, "/var/log/%s/", tag);
#endif
    (void)mkdir(_log_path, 0777);
  }

  return ((const char *)_log_path);
}

void zlog(int level, int code, char *data)
{
	printf ("DEBUG: level(%d) code(%d) data(%s)\n", level, code, data);
}


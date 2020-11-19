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

#ifndef __ZSYS_LOG_H__
#define __ZSYS_LOG_H__

#ifdef __cplusplus
extern "C" {
#endif

#define ZERR_INFO -199

#define ZLOG_NONE 0
#define ZLOG_INFO 1
#define ZLOG_WARNING 2
#define ZLOG_ERROR 3
#define ZLOG_RUSAGE 4
#define MAX_ZLOG_LEVEL 5

#define zlog_error(_err, _text) \
  (f_zsys_log(_err, "error", _text, __FILE__, __LINE__))

#define zlog_debug(_text) \
  (f_zsys_log(ZERR_INFO, "debug", _text, __FILE__, __LINE__))

#define zlog_info(_text) \
  (f_zsys_log(ZERR_INFO, "info", _text, __FILE__, __LINE__))

void zlog(int level, int code, char *text);

void f_zsys_log(int err_code, const char *tag, const char *text, const char *src_fname, long src_line);

#ifdef __cplusplus
}
#endif

#endif /* ndef __ZSYS_LOG_H__ */

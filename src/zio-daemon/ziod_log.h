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

#ifndef __ZIOD__LOG_H__
#define __ZIOD__LOG_H__

#ifdef __cplusplus
extern "C" {
#endif

#define ZLOG_NONE 0
#define ZLOG_INFO 1
#define ZLOG_WARNING 2
#define ZLOG_ERROR 3
#define MAX_ZLOG_LEVEL 4

#define ZERR_INFO -199

#define zio_log(_text) \
  (f_zio_log(0, "Info", _text, __FILE__, __LINE__))

#define zio_error(_err, _tag, _text) \
  (f_zio_log(_err, _tag, _text, __FILE__, __LINE__))

#define zio_info(_tag, _text) \
  (f_zio_log(ZERR_INFO, _tag, _text, __FILE__, __LINE__))

#define zio_netlog(_node, _cmd) \
	(f_zio_log_net(GetCoinByIndex( \
			(_node)->ifaceIndex)->name, (_node)->addr.ToString().c_str(), \
			(_cmd)->pchCommand, (_cmd)->nMessageSize, __FILE__, __LINE__))

void f_zio_log(int err_code, const char *tag, const char *text, const char *src_fname, long src_line);

void f_zio_log_net(const char *iface, const char *addr, const char *tag, size_t size, const char *src_fname, long src_line);

const char *zio_log_path(void);

void zlog(int level, int code, char *data);

#ifdef __cplusplus
}
#endif

#endif /* ndef __ZIOD__LOG_H__ */

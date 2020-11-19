
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

#ifndef __CORE_OPT_H__
#define __CORE_OPT_H__


#ifdef __cplusplus
extern "C" {
#endif

#define OPT_DEBUG "debug"
#define OPT_NET_DEBUG "net-debug"
#define OPT_MAX_CONN "net-max"
#define OPT_ZIO_PORT "zio-port"
//#define OPT_ZIO_KEY "zio-key"

void opt_init(void);

void opt_term(void);

int opt_num(const char *tag);

void opt_num_set(const char *tag, int num);

const char *opt_str(const char *tag);

void opt_str_set(const char *tag, char *str);

int opt_bool(const char *tag);

void opt_bool_set(const char *tag, int b);

const char *opt_usage_print(void);

void opt_arg_interp(int argc, char **argv);

const char *opt_config_default_print(void);

const char *get_zio_data_path(void);

void opt_print(void);

#ifdef __cplusplus
}
#endif


#endif /* ndef __CORE_OPT_H__ */


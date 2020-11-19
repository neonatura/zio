
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

#define OPT_TYPE_NULL 0
#define OPT_TYPE_BOOL 1
#define OPT_TYPE_NUM 2
#define OPT_TYPE_STR 3

typedef struct opt_t {
	const char *opt_name;
	int opt_type;
	int64_t opt_def;
	const char *opt_strdef;
	const char *opt_desc;
} opt_t;

static hmap_t *_proc_option_table;
#define OPT_LIST _proc_option_table

static opt_t _option_table[] = {
	{ OPT_DEBUG, OPT_TYPE_BOOL, 0, NULL,
		"Verbose logging information." },
	{ OPT_NET_DEBUG, OPT_TYPE_BOOL, 0, NULL,
		"Verbose network command logging." },
	{ OPT_MAX_CONN, OPT_TYPE_NUM, 300, NULL,
		"The maximum number of socket connections allowed for all services." }, 

	/* network options */
	{ OPT_ZIO_PORT, OPT_TYPE_NUM, 22104, NULL,
		"The port to accept incoming zio service connections." },

#if 0
  { OPT_ZIO_KEY, OPT_TYPE_STR, 0, "",
    "A hex key for authenticating zio service requests." },
#endif

	{ "", OPT_TYPE_NULL, 0, "" },

};


static const char *opt_home_dir(void)
{
	static char ret_buf[PATH_MAX+1];

	if (!*ret_buf) {
		char* homedir = NULL;
		struct passwd *pw = NULL;

#ifdef HAVE_GETPWUID
		pw = getpwuid(getuid());
		if (pw)
			homedir = pw->pw_dir;
#endif

		if (!homedir) {
#ifdef WINDOWS
			/* kick back to windows set env. var. note that this is probably not defined for a 'system account'. */
			homedir = getenv("HOMEPATH");
#else
			homedir = getenv("HOME");
#endif
		}
		if (homedir) {
			strncpy(ret_buf, homedir, sizeof(ret_buf)-1);
#ifdef WINDOWS
			if (*ret_buf && ret_buf[strlen(ret_buf)-1] == '\\')
				ret_buf[strlen(ret_buf)-1] = '\000';
#else
			if (*ret_buf && ret_buf[strlen(ret_buf)-1] == '/')
				ret_buf[strlen(ret_buf)-1] = '\000';
#endif
		}
	}

	return ((const char *)ret_buf);
}

void opt_print(void)
{
  char buf[512];
	int idx;

	for (idx = 0; _option_table[idx].opt_type != OPT_TYPE_NULL; idx++) {
		switch (_option_table[idx].opt_type) {
			case OPT_TYPE_BOOL:
				sprintf(buf, "%s set to \"%s\".", 
						_option_table[idx].opt_name,
						opt_bool(_option_table[idx].opt_name) ? "true" : "false");
				zlog_info(buf); 
				break;
			case OPT_TYPE_NUM:
				sprintf(buf, "%s set to \"%d\".", 
						_option_table[idx].opt_name,
						opt_num(_option_table[idx].opt_name));
				zlog_info(buf); 
				break;
			case OPT_TYPE_STR:
				sprintf(buf, "%s set to \"%s\".", 
						_option_table[idx].opt_name,
						opt_str(_option_table[idx].opt_name));
				zlog_info(buf); 
				break;
		}
	}

}

/** Write out the defaults to "zio.conf". */ 
static void write_default_zio_conf_file(char *path)
{
	FILE *fl;
	const char *data;
	size_t data_len;
	int err;

	data = opt_config_default_print();
	if (!data)
		return;

	/* write template to disk */
	data_len = strlen(data);
	fl = fopen(path, "wb");
	if (!fl)
		return;
	(void)fwrite(data, data_len, sizeof(uint8_t), fl);
	(void)fclose(fl);
}

static int opt_set_defaults_datfile(char *path)
{
	struct stat st;
	FILE *fl;
	char *tok, *val;
	char *data;
	char *line;
	size_t data_len;
	int idx;
	int err;

	err = fstat(path, &st);
	if (err)
		return (zsyserror());

	fl = fopen(path, "rb");
	if (!fl)
		return (zsyserror());

	/* read file into memory */
	data_len = st.st_size;
	data = (char *)calloc(data_len, sizeof(uint8_t));
	if (!data) return (zsyserror());
	(void)fread(data, data_len, sizeof(uint8_t), fl);
	(void)fclose(fl);

	line = strtok(data, "\r\n");
	while (line) {
		if (!*line) goto next;
		if (*line == '#') goto next;

		tok = line;
		val = strchr(line, '=');
		if (!val) goto next;
		*val++ = '\000';

		for (idx = 0; _option_table[idx].opt_type != OPT_TYPE_NULL; idx++) {
			if (0 == strcasecmp(tok, _option_table[idx].opt_name)) {
				switch (_option_table[idx].opt_type) {
					case OPT_TYPE_BOOL:
						opt_bool_set(_option_table[idx].opt_name, atoi(val));
						break;
					case OPT_TYPE_NUM:
						opt_num_set(_option_table[idx].opt_name, atoi(val));
						break;
					case OPT_TYPE_STR:
						opt_str_set(_option_table[idx].opt_name, val);
						break;
				}
				break;
			}
		}

next:
		line = strtok(NULL, "\r\n");
	}

	return (0);
}

static const char *opt_set_environment_name(opt_t *opt)
{
	static char env_buf[1024];
	int len;
	int i;

	sprintf(env_buf, "SHCOIND_%s", opt->opt_name);

	len = strlen(env_buf);
	for (i = 0; i < len; i++) {
		if (ispunct(env_buf[i]))
			env_buf[i] = '_';
		else 
			env_buf[i] = toupper(env_buf[i]);
	}

	return (env_buf);
}

static void opt_set_environment_settings(void)
{
	char *env;
	int idx;

	for (idx = 0; _option_table[idx].opt_type != OPT_TYPE_NULL; idx++) {
		env = getenv(opt_set_environment_name(&_option_table[idx]));
		if (!env) continue;
		switch (_option_table[idx].opt_type) {
			case OPT_TYPE_BOOL:
				opt_bool_set(_option_table[idx].opt_name, !!atoi(env));
				break;
			case OPT_TYPE_NUM:
				opt_num_set(_option_table[idx].opt_name, atoi(env));
				break;
			case OPT_TYPE_STR:
				opt_str_set(_option_table[idx].opt_name, env);
				break;
		}
	}

}

static void opt_set_defaults(void)
{
	char path[PATH_MAX+1];
	int idx;
	int err;

	/* hard-coded configurable defaults */
	for (idx = 0; _option_table[idx].opt_type != OPT_TYPE_NULL; idx++) {
		switch (_option_table[idx].opt_type) {
			case OPT_TYPE_BOOL:
				opt_bool_set(_option_table[idx].opt_name, _option_table[idx].opt_def);
				break;
			case OPT_TYPE_NUM:
				opt_num_set(_option_table[idx].opt_name, _option_table[idx].opt_def);
				break;
			case OPT_TYPE_STR:
				opt_str_set(_option_table[idx].opt_name, _option_table[idx].opt_strdef);
				break;
		}
	}

	/* "<data dir>/zio.conf" datafile */
	sprintf(path, "%szio.conf", zsys_data_path());
	err = opt_set_defaults_datfile(path);
	if (err)
		write_default_zio_conf_file(path);

	/* secondary "~/.zio/zio.conf" datafile */
#ifdef WINDOWS
	sprintf(path, "%s\\.zio\\zio.conf", opt_home_dir());
#else
	sprintf(path, "%s/.zio/zio.conf", opt_home_dir());
#endif
	opt_set_defaults_datfile(path);

	/* environment settings */
	opt_set_environment_settings();

}

static uint64_t tag_key(char *tag)
{
	return (crc64(0, tag, strlen(tag)));
}

void opt_init(void)
{
  OPT_LIST = hmap_init();

  opt_set_defaults();
}

void opt_term(void)
{

  if (!OPT_LIST)
    return;

  hmap_free(&OPT_LIST);
  OPT_LIST = NULL;
}

int opt_num(const char *tag)
{
  void *v = hmap_get(OPT_LIST, tag_key(tag));
  return ((int)(uint64_t)v);
}

void opt_num_set(const char *tag, int num)
{
  void *v = (void *)(uint64_t)num;
  hmap_set(OPT_LIST, tag_key(tag), v);
}

const char *opt_str(const char *tag)
{
  char *str = (char *)hmap_get(OPT_LIST, tag_key(tag));
  return ((const char *)str);
}

void opt_str_set(const char *tag, char *str)
{
	hmap_set(OPT_LIST, tag_key(tag), strdup(str)); 
}

int opt_bool(const char *tag)
{
  int b = opt_num(tag) ? TRUE : FALSE;
  return (b);
}

void opt_bool_set(const char *tag, int b)
{
  opt_num_set(tag, b ? TRUE : FALSE);
}

const char *opt_usage_print(void)
{
	static buff_t *ret_buff;
  char buf[512];
	int idx;

	if (!ret_buff)
		ret_buff = buff_init();
	else
		buff_clear(ret_buff);

	for (idx = 0; _option_table[idx].opt_type != OPT_TYPE_NULL; idx++) {
		memset(buf, 0, sizeof(buf));
		switch (_option_table[idx].opt_type) {
			case OPT_TYPE_BOOL:
				if (_option_table[idx].opt_def) {
					sprintf(buf, "\t--no-%s\n\t\t%s\n\n",
							_option_table[idx].opt_name, _option_table[idx].opt_desc);
				} else {
					sprintf(buf, "\t--%s\n\t\t%s\n\n", 
							_option_table[idx].opt_name, _option_table[idx].opt_desc);
				}
				break;
			case OPT_TYPE_NUM:
				sprintf(buf, "\t--%s=%d\n\t\t%s\n\n", 
						_option_table[idx].opt_name, 
						_option_table[idx].opt_def,
						_option_table[idx].opt_desc);
				break;
			case OPT_TYPE_STR:
				sprintf(buf, "\t--%s=%s\n\t\t%s\n\n", 
						_option_table[idx].opt_name, 
						_option_table[idx].opt_strdef,
						_option_table[idx].opt_desc);
				break;
		}
		buff_catstr(ret_buff, buf); 
	}

	return ((const char *)buff_data(ret_buff));
}

void opt_arg_interp(int argc, char **argv)
{
	char buf[256];
	char *opt_name;
	char *tok, *val;
	char *ptr;
	int num_val;
	int bool_val;
	int idx;
	int i;

	for (i = 1; i < argc; i++) {
		if (0 != strncmp(argv[i], "--", 2))
			continue;

		memset(buf, 0, sizeof(buf));
		strncpy(buf, argv[i] + 2, sizeof(buf)-1);

		tok = buf;
		val = strchr(buf, '=');
		if (val)
			*val++ = '\000';

		num_val = 0;
		bool_val = 1;
		if (0 == strncmp(tok, "no-", 3)) {
			bool_val = 0;
			tok += 3;
		}
		if (val)
			num_val = atoi(val);

		for (idx = 0; _option_table[idx].opt_type != OPT_TYPE_NULL; idx++) {
			if (0 == strcasecmp(_option_table[idx].opt_name, tok)) {
				switch (_option_table[idx].opt_type) {
					case OPT_TYPE_BOOL:
						opt_bool_set(_option_table[idx].opt_name, bool_val);
						break;
					case OPT_TYPE_NUM:
						opt_num_set(_option_table[idx].opt_name, num_val);
						break;
					case OPT_TYPE_STR:
						opt_str_set(_option_table[idx].opt_name, val?val:"");
						break;
				}
				break;
			}
		}
	}

}

const char *opt_config_default_print(void)
{
	static buff_t *ret_buff;
  char buf[1024];
	int idx;

	if (!ret_buff)
		ret_buff = buff_init();
	else
		buff_clear(ret_buff);

	for (idx = 0; _option_table[idx].opt_type != OPT_TYPE_NULL; idx++) {
		memset(buf, 0, sizeof(buf));
		switch (_option_table[idx].opt_type) {
			case OPT_TYPE_BOOL:
				sprintf(buf, 
						"\n"
						"## Name: \"%s\"\n"
						"## %s\n",
						_option_table[idx].opt_name,
						_option_table[idx].opt_desc);
				if (_option_table[idx].opt_def) {
					sprintf(buf+strlen(buf), "## Default: 1 (true)\n#%s=1\n",
							_option_table[idx].opt_name);
				} else {
					sprintf(buf+strlen(buf), "## Default: 0 (false)\n#%s=0\n",
							_option_table[idx].opt_name);
				}
				break;
			case OPT_TYPE_NUM:
				sprintf(buf,
						"\n"
						"## Name: \"%s\"\n"
						"## %s\n"
						"## Default: %d\n"
						"#%s=%d\n",
						_option_table[idx].opt_name,
						_option_table[idx].opt_desc,
						_option_table[idx].opt_def,
						_option_table[idx].opt_name, 
						_option_table[idx].opt_def);
				break;
			case OPT_TYPE_STR:
				sprintf(buf, 
						"\n"
						"## Name: \"%s\"\n"
						"## %s\n",
						_option_table[idx].opt_name,
						_option_table[idx].opt_desc);
				if (_option_table[idx].opt_strdef &&
						*_option_table[idx].opt_strdef) {
					sprintf(buf+strlen(buf), "## Default: %s\n",
							_option_table[idx].opt_strdef);
				}
				if (_option_table[idx].opt_strdef &&
						0 == strcmp(_option_table[idx].opt_strdef, 
							opt_str(_option_table[idx].opt_name))) {
					sprintf(buf+strlen(buf), "#%s=%s\n", 
							_option_table[idx].opt_name,
							_option_table[idx].opt_strdef);
				} else {
					sprintf(buf+strlen(buf), "%s=%s\n", 
							_option_table[idx].opt_name, 
							opt_str(_option_table[idx].opt_name));
				}
				break;
		}
		buff_catstr(ret_buff, buf); 
	}

	return ((const char *)buff_data(ret_buff));
}


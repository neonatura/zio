
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

const char *zsys_data_path(void)
{
  static char ret_path[PATH_MAX+1];

  if (!*ret_path) {
#ifdef WINDOWS
    char *str;

    str = getenv("ProgramData");
    if (!str)
      str = "C:\\ProgramData";

    sprintf(ret_path, "%s\\zio\\", str);
    mkdir(ret_path, 0777);
#else
    strcpy(ret_path, "/var/lib/zio/");
    mkdir(ret_path, 0777);
#endif
  }

  return (ret_path);
}

const char *zsys_log_path(void)
{
  static char ret_path[PATH_MAX+1];
	const char *tag = "zio";

	if (!*ret_path) {
		if (!tag)
			tag = "share";

		/* default log directory */
#ifdef WINDOWS
		sprintf(ret_path, "%s\\%s\\", getenv("ProgramData"), tag);
#else
		sprintf(ret_path, "/var/log/%s/", tag);
#endif
		(void)mkdir(ret_path, 0777);
	}

	return ((const char *)ret_path);
}


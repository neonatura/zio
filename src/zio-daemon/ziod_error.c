
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

static err_code_t _error_code_table[] = {
  /* reserved */
  { ZERR_UNKNOWN, "Unknown error" },

  /* posix */
  { ZERR_NONE, "NONE" },
  { ZERR_NOENT, "entity not found" },
  { ZERR_SRCH, "SRCH" },
  { ZERR_IO, "IO" },
  { ZERR_2BIG, "2BIG" },
  { ZERR_NOEXEC, "executable format error" },
  { ZERR_BADF, "BADF" },
  { ZERR_AGAIN, "resource temporarily unavailable" },
  { ZERR_NOMEM, "unable to allocate memory" },
  { ZERR_ACCESS, "permission denied" },
  { ZERR_EXIST, "entity already exists" },
  { ZERR_NOTDIR, "NOTDIR" },
  { ZERR_ISDIR, "ISDIR" },
  { ZERR_INVAL, "invalid parameter" },
  { ZERR_NFILE, "NFILE" },
  { ZERR_FBIG, "FBIG" },
  { ZERR_NOSPC, "NOSPC" },
  { ZERR_NAMETOOLONG, "NAMETOOLONG" },
  { ZERR_NOLCK, "NOLCK" },
  { ZERR_NOMSG, "NOMSG" },
  { ZERR_XFULL, "XFULL" },
  { ZERR_OVERFLOW, "OVERFLOW" },
  { ZERR_NOTUNIQ, "key is not unique" },
  { ZERR_ILSEQ, "illegal byte sequence" },
  { ZERR_CONNRESET, "CONNRESET" },
  { ZERR_NOBUFS, "NOBUFS" },
  { ZERR_TIMEDOUT, "timeout" },
  { ZERR_CONNREFUSED, "connection refused" },
  { ZERR_ALREADY, "ALREADY" },
  { ZERR_REMOTEIO, "REMOTEIO" },
  { ZERR_TIME, "TIME" },
  { ZERR_NONET, "NONET" },
  { ZERR_NOPKG, "NOPKG" },
  { ZERR_REMOTE, "remote resource" },
  { ZERR_NOLINK, "NOLINK" },
  { ZERR_PROTO, "protocol error" },
  { ZERR_NOTSOCK, "NOTSOCK" },
  { ZERR_OPNOTSUPP, "operation not supported" },
  { ZERR_ADDRINUSE, "ADDRINUSE" },
  { ZERR_ADDRNOTAVAIL, "ADDRNOTAVAIL" },
  { ZERR_NETDOWN, "NETDOWN" },
  { ZERR_NETUNREACH, "NETUNREACH" },
  { ZERR_SHUTDOWN, "SHUTDOWN" },
  { ZERR_TOOMANYREFS, "TOOMANYREFS" },
  { ZERR_INPROGRESS, "INPROGRESS" },
  { ZERR_NOMEDIUM, "NOMEDIUM" },
  { ZERR_CANCELED, "CANCELED" },
  { ZERR_NOKEY, "key not available" },
  { ZERR_KEYEXPIRED, "expired key" },
  { ZERR_KEYREVOKED, "revoked key" },
  { ZERR_KEYREJECTED, "rejected key" },
  { ZERR_OWNERDEAD, "OWNERDEAD" },

  /* custom */
	{ ZERR_ENCODE, "Serialization failure" },

	/* terminator */
  { 0, NULL },
};

const char *zio_strerror(int code)
{
  static const char *unknown_error_str = "Unknown error";
  int i;

  if (code > 0)
    code *= -1;

  for (i = 0; _error_code_table[i].label; i++) {
    if (_error_code_table[i].code == code)
      return (_error_code_table[i].label);
  }

  return (unknown_error_str);
}

int zio_syserror(void)
{
	return (-1);
}

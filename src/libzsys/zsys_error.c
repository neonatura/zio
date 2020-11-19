/*
 * @copyright
 *
 *  Copyright 2018 Neo Natura
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

static err_code_t _error_code_table[] = {
  /* reserved */
  { ERR_UNKNOWN, "Unknown error" },

  /* posix */
  { ERR_NONE, "NONE" },
  { ERR_NOENT, "entity not found" },
  { ERR_SRCH, "SRCH" },
  { ERR_IO, "IO" },
  { ERR_2BIG, "2BIG" },
  { ERR_NOEXEC, "executable format error" },
  { ERR_BADF, "BADF" },
  { ERR_AGAIN, "resource temporarily unavailable" },
  { ERR_NOMEM, "unable to allocate memory" },
  { ERR_ACCESS, "permission denied" },
  { ERR_EXIST, "entity already exists" },
  { ERR_NOTDIR, "NOTDIR" },
  { ERR_ISDIR, "ISDIR" },
  { ERR_INVAL, "invalid parameter" },
  { ERR_NFILE, "NFILE" },
  { ERR_FBIG, "FBIG" },
  { ERR_NOSPC, "NOSPC" },
  { ERR_NAMETOOLONG, "NAMETOOLONG" },
  { ERR_NOLCK, "NOLCK" },
  { ERR_NOMSG, "NOMSG" },
  { ERR_XFULL, "XFULL" },
  { ERR_OVERFLOW, "OVERFLOW" },
  { ERR_NOTUNIQ, "key is not unique" },
  { ERR_ILSEQ, "illegal byte sequence" },
  { ERR_CONNRESET, "CONNRESET" },
  { ERR_NOBUFS, "NOBUFS" },
  { ERR_TIMEDOUT, "timeout" },
  { ERR_CONNREFUSED, "connection refused" },
  { ERR_ALREADY, "ALREADY" },
  { ERR_REMOTEIO, "REMOTEIO" },
  { ERR_TIME, "TIME" },
  { ERR_NONET, "NONET" },
  { ERR_NOPKG, "NOPKG" },
  { ERR_REMOTE, "remote resource" },
  { ERR_NOLINK, "NOLINK" },
  { ERR_PROTO, "protocol error" },
  { ERR_NOTSOCK, "NOTSOCK" },
  { ERR_OPNOTSUPP, "operation not supported" },
  { ERR_ADDRINUSE, "ADDRINUSE" },
  { ERR_ADDRNOTAVAIL, "ADDRNOTAVAIL" },
  { ERR_NETDOWN, "NETDOWN" },
  { ERR_NETUNREACH, "NETUNREACH" },
  { ERR_SHUTDOWN, "SHUTDOWN" },
  { ERR_TOOMANYREFS, "TOOMANYREFS" },
  { ERR_INPROGRESS, "INPROGRESS" },
  { ERR_NOMEDIUM, "NOMEDIUM" },
  { ERR_CANCELED, "CANCELED" },
  { ERR_NOKEY, "key not available" },
  { ERR_KEYEXPIRED, "expired key" },
  { ERR_KEYREVOKED, "revoked key" },
  { ERR_KEYREJECTED, "rejected key" },
  { ERR_OWNERDEAD, "OWNERDEAD" },

  /* custom */
	{ ERR_ENCODE, "Serialization failure" },

	/* terminator */
  { 0, NULL }
};

syserr_code_t _syserror_code_table[] = {
	{ ERR_NOENT, ENOENT },
	{ ERR_SRCH, ESRCH },

	{ 0, 0 }
};

const char *zstrerror(int code)
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

int zsyserror(void)
{
	const int code = errno;
  int i;

  for (i = 0; _syserror_code_table[i].code; i++) {
    if (_syserror_code_table[i].syscode == code)
      return (_syserror_code_table[i].code);
  }

  return (ERR_UNKNOWN);
}

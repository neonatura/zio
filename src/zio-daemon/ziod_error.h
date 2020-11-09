
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

#ifndef __CORE_ERROR_H__
#define __CORE_ERROR_H__


/* reserved */
#define ZERR_UNKNOWN -1

/* custom (non posix) errors */
#define ERR_MASK -1000
#define CUSTOM_ERROR(_code) \
  (ERR_MASK - (_code))

/* posix based errors */
#define ZERR_NONE 0
#define ZERR_NOENT -2 /* ENOENT */
#define ZERR_SRCH -3 /* ESRCH */
#define ZERR_IO -5 /* EIO */
#define ZERR_2BIG -7 /* E2BIG */
#define ZERR_NOEXEC -8 /* NOEXEC */
#define ZERR_BADF -9 /* EBADF */
#define ZERR_AGAIN -11 /* EAGAIN */
#define ZERR_NOMEM -12 /* ENOMEM */
#define ZERR_ACCESS -13 /* EACCES */
#define ZERR_EXIST -17 /* EEXIST */
#define ZERR_NOTDIR -20 /* ENOTDIR */
#define ZERR_ISDIR -21 /* EISDIR */
#define ZERR_INVAL -22 /* EINVAL */
#define ZERR_NFILE -23  /* ENFILE (File table overflow) */
#define ZERR_FBIG -27 /* EFBIG */
#define ZERR_NOSPC -28 /* ENOSPC (No space left on device) */
#define ZERR_NAMETOOLONG -36 /* ENAMETOOLONG (File name too long) */
#define ZERR_NOLCK -37 /* No record locks available */
#define ZERR_NOMSG -42 /* ENOMSG */
#define ZERR_XFULL -54 /* EXFULL (Exchange full) */
#define ZERR_OVERFLOW -75 /* Value too large for defined value type */
#define ZERR_NOTUNIQ -76 /* Name not unique on network */
#define ZERR_ILSEQ -84 /* Illegal [byte] sequence. */
#define ZERR_CONNRESET -104 /* Connection reset by peer. */
#define ZERR_NOBUFS -105 /* No buffer space available. */
#define ZERR_TIMEDOUT -110 /* Conenction timed out */
#define ZERR_CONNREFUSED -111 /* Conenction refused */
#define ZERR_ALREADY -114 /* Operation already in progress */
#define ZERR_REMOTEIO -121 /* EREMOTEIO */
#define ZERR_TIME -62 /* Timer expired */
#define ZERR_NONET -64  /* Machine is not on the network */
#define ZERR_NOPKG -65  /* Package not installed */
#define ZERR_REMOTE -66  /* Object is remote */
#define ZERR_NOLINK -67  /* Link has been severed */
#define ZERR_PROTO -71 /* Protocol error */
#define ZERR_NOTSOCK -88  /* Socket operation on non-socket */
#define ZERR_OPNOTSUPP -95 /* Operation not supported */
#define ZERR_ADDRINUSE -98  /* Address already in use */
#define ZERR_ADDRNOTAVAIL -99  /* Cannot assign requested address */
#define ZERR_NETDOWN -100 /* Network is down */
#define ZERR_NETUNREACH -101 /* Network is unreachable */
#define ZERR_SHUTDOWN -108 /* Cannot send after transport endpoint shutdown */
#define ZERR_TOOMANYREFS -109 /* Too many references: cannot splice */
#define ZERR_INPROGRESS -115 /* Operation now in progress */
#define ZERR_NOMEDIUM -123 /* No medium found */
#define ZERR_CANCELED -125
#define ZERR_NOKEY -126
#define ZERR_KEYEXPIRED -127
#define ZERR_KEYREVOKED -128
#define ZERR_KEYREJECTED -129
#define ZERR_OWNERDEAD -130

/* custon zio errors */
#define ZERR_ENCODE     CUSTOM_ERROR(1)

typedef struct err_code_t
{
  int code;
  const char *label;
} err_code_t;

const char *zio_strerror(int code);

/* return the ZIO error code related to the current system error (i.e. errno) */
int zio_syserror(void);

#endif /* ndef __CORE_ERROR_H__ */


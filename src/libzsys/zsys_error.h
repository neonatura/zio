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

#ifndef __HTM_ERROR_H__
#define __HTM_ERROR_H__

/* reserved */
#define ERR_UNKNOWN -1

/* custom (non posix) errors */
#define ERR_MASK -1000
#define CUSTOM_ERROR(_code) \
  (ERR_MASK - (_code))

/* posix based errors */
#define ERR_NONE 0
#define ERR_NOENT -2 /* ENOENT */
#define ERR_SRCH -3 /* ESRCH */
#define ERR_IO -5 /* EIO */
#define ERR_2BIG -7 /* E2BIG */
#define ERR_NOEXEC -8 /* NOEXEC */
#define ERR_BADF -9 /* EBADF */
#define ERR_AGAIN -11 /* EAGAIN */
#define ERR_NOMEM -12 /* ENOMEM */
#define ERR_ACCESS -13 /* EACCES */
#define ERR_EXIST -17 /* EEXIST */
#define ERR_NOTDIR -20 /* ENOTDIR */
#define ERR_ISDIR -21 /* EISDIR */
#define ERR_INVAL -22 /* EINVAL */
#define ERR_NFILE -23  /* ENFILE (File table overflow) */
#define ERR_FBIG -27 /* EFBIG */
#define ERR_NOSPC -28 /* ENOSPC (No space left on device) */
#define ERR_NAMETOOLONG -36 /* ENAMETOOLONG (File name too long) */
#define ERR_NOLCK -37 /* No record locks available */
#define ERR_NOMSG -42 /* ENOMSG */
#define ERR_XFULL -54 /* EXFULL (Exchange full) */
#define ERR_OVERFLOW -75 /* Value too large for defined value type */
#define ERR_NOTUNIQ -76 /* Name not unique on network */
#define ERR_ILSEQ -84 /* Illegal [byte] sequence. */
#define ERR_CONNRESET -104 /* Connection reset by peer. */
#define ERR_NOBUFS -105 /* No buffer space available. */
#define ERR_TIMEDOUT -110 /* Conenction timed out */
#define ERR_CONNREFUSED -111 /* Conenction refused */
#define ERR_ALREADY -114 /* Operation already in progress */
#define ERR_REMOTEIO -121 /* EREMOTEIO */
#define ERR_TIME -62 /* Timer expired */
#define ERR_NONET -64  /* Machine is not on the network */
#define ERR_NOPKG -65  /* Package not installed */
#define ERR_REMOTE -66  /* Object is remote */
#define ERR_NOLINK -67  /* Link has been severed */
#define ERR_PROTO -71 /* Protocol error */
#define ERR_NOTSOCK -88  /* Socket operation on non-socket */
#define ERR_OPNOTSUPP -95 /* Operation not supported */
#define ERR_ADDRINUSE -98  /* Address already in use */
#define ERR_ADDRNOTAVAIL -99  /* Cannot assign requested address */
#define ERR_NETDOWN -100 /* Network is down */
#define ERR_NETUNREACH -101 /* Network is unreachable */
#define ERR_SHUTDOWN -108 /* Cannot send after transport endpoint shutdown */
#define ERR_TOOMANYREFS -109 /* Too many references: cannot splice */
#define ERR_INPROGRESS -115 /* Operation now in progress */
#define ERR_NOMEDIUM -123 /* No medium found */
#define ERR_CANCELED -125
#define ERR_NOKEY -126
#define ERR_KEYEXPIRED -127
#define ERR_KEYREVOKED -128
#define ERR_KEYREJECTED -129
#define ERR_OWNERDEAD -130

/* custon error codes */
#define ERR_ENCODE     CUSTOM_ERROR(1)

typedef struct err_code_t
{
  int code;
  const char *label;
} err_code_t;

typedef struct syserr_code_t
{
  int code;
  int syscode;
} syserr_code_t;

const char *zstrerror(int code);

/* return the HTM error code related to the current system error (i.e. errno) */
int zsyserror(void);

#endif /* ndef __HTM_ERROR_H__ */

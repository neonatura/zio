#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([shared], [3.02])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/core])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

LT_INIT

version_patch=2
AC_SUBST(version_patch)
version_info=3:2:0
AC_SUBST(version_info)


# Architecture
AC_MSG_CHECKING([for supported operating system])
case "$host_os" in
  *linux*)
  opsys=linux
  AC_DEFINE([LINUX], 1, [Define for the Linux operating system.])
  AC_DEFINE([POLL_NFDS_TYPE],[nfds_t],[type of second poll() argument])
  AC_CHECK_HEADERS([poll.h])
  AC_CHECK_TYPE([nfds_t],
    [AC_DEFINE([POLL_NFDS_TYPE],[nfds_t],[type of second poll() argument])],
    [AC_DEFINE([POLL_NFDS_TYPE],[unsigned int],[type of second poll() argument])],
    [#include <poll.h>])
  ;;
  windows)
  opsys=win
  AC_DEFINE([WINDOWS], 1, [Define for the Windows operating system.])
  AC_DEFINE([POLL_NFDS_TYPE],[unsigned int],[type of second poll() argument])
  ;;
  mingw*)
  opsys=win
AC_DEFINE([WINDOWS], 1, [Define for the Windows operating system.])
  ;;
  msys*)
  opsys=win
AC_DEFINE([WINDOWS], 1, [Define for the Windows operating system.])
  ;;
  sunos4*)
  opsys=sunos4
AC_DEFINE([SUNOS4], 1, [Define for the SunOS 4.x operating system.])
  ;;
  solaris2* | sysv[[45]]* | irix[[56]]*)
  opsys=svr4
  AC_DEFINE([SVR4], 1, [Define for the System V release 4 operating
      system or a derivative like Solaris 2.x or Irix 5.x.])
  case "$host_os" in
  sysv4.2uw*)
AC_DEFINE(UNIXWARE, 2, [Define for UnixWare systems.])
  ;;
  sysv5*)
AC_DEFINE(UNIXWARE, 7, [Define for UnixWare systems.])
  ;;
  esac
  ;;
  freebsd*)
  opsys=freebsd
AC_DEFINE([FREEBSD], 1, [Define for the FreeBSD operating system.])
  ;;
  *)
  AC_MSG_RESULT([NO!])
AC_MSG_ERROR([operating system $host_os is not supported.])
  ;;
  esac
  AC_MSG_RESULT($opsys)
AC_MSG_CHECKING([for supported architecture])
  case "$host_cpu" in
  bfin)
  arch=bfin
AC_DEFINE([BFIN], 1, [Define for the Blackfin architecture.])
  ;;
  i[[3456]]86|pentium)
  arch=i386
AC_DEFINE([I386], 1, [Define for the i386 architecture.])
  ;;
  mingw32)
  arch=i386
AC_DEFINE([I386], 1, [Define for the i386 architecture.])
  ;;
  ia64)
  arch=ia64
AC_DEFINE([IA64], 1, [Define for the IA64 architecture.])
  ;;
  m68k)
  arch=m68k
AC_DEFINE([M68K], 1, [Define for the m68k architecture.])
  ;;
  sparc64*)
  arch=sparc64
AC_DEFINE([SPARC64], 1, [Define for the SPARC64 architecture.])
  ;;
  sparc*)
  arch=sparc
AC_DEFINE([SPARC], 1, [Define for the SPARC architecture.])
  ;;
  mips*)
  arch=mips
AC_DEFINE([MIPS], 1, [Define for the MIPS architecture.])
  ;;
  alpha*)
  arch=alpha
AC_DEFINE([ALPHA], 1, [Define for the Alpha architecture.])
  ;;
  powerpc*)
  arch=powerpc
AC_DEFINE([POWERPC], 1, [Define for the PowerPC architecture.])
  ;;
  arm*)
  arch=arm
AC_DEFINE([ARM], 1, [Define for the ARM architecture.])
  ;;
  avr32*)
  arch=avr32
AC_DEFINE([AVR32], 1, [Define for the AVR32 architecture.])
  ;;
  s390)
  arch=s390
AC_DEFINE([S390], 1, [Define for the S390 architecture.])
  ;;
  s390x)
  arch=s390x
AC_DEFINE([S390X], 1, [Define for the S390x architecture.])
  ;;
  hppa*|parisc*)
  arch=hppa
AC_DEFINE([HPPA], 1, [Define for the HPPA architecture.])
  ;;
  sh64*)
  arch=sh64
AC_DEFINE([SH64], 1, [Define for the SH64 architecture.])
  ;;
  sh*)
  arch=sh
AC_DEFINE([SH], 1, [Define for the SH architecture.])
  ;;
  x86?64*)
  arch=x86_64
AC_DEFINE([X86_64], 1, [Define for the AMD x86-64 architecture.])
  ;;
  mingw64)
  arch=x86_64
AC_DEFINE([X86_64], 1, [Define for the AMD x86-64 architecture.])
  ;;
  cris|crisv10)
  arch=crisv10
AC_DEFINE([CRISV10], 1, [Define for the CRISv10 architecture.])
  ;;
  crisv32)
  arch=crisv32
AC_DEFINE([CRISV32], 1, [Define for the CRISv32 architecture.])
  ;;
  tile*)
  arch=tile
AC_DEFINE([TILE], 1, [Define for the Tile architecture])
  ;;
  *)
  AC_MSG_RESULT([NO!])
AC_MSG_ERROR([architecture $host_cpu is not supported.])
  ;;
  esac
AC_MSG_RESULT($arch)
AC_SUBST(opsys)
AC_SUBST(arch)
AM_CONDITIONAL([LINUX], [test x$opsys = xlinux])
AM_CONDITIONAL([WINDOWS], [test x$opsys = xwin])
AM_CONDITIONAL([I386], [test x$arch = xi386])
AM_CONDITIONAL([X86_64], [test x$arch = xx86_64])
AM_CONDITIONAL([SUNOS4], [test x$opsys = xsunos4])
AM_CONDITIONAL([SVR4], [test x$opsys = xsvr4])
AM_CONDITIONAL([FREEBSD], [test x$opsys = xfreebsd])


# Checks for programs.
AC_PROG_CC
gl_EARLY

AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_LIBTOOL
AM_PROG_CC_C_O
AC_ISC_POSIX
AC_C_INLINE


# Checks for header files.
AC_CHECK_HEADERS(stdlib.h stdint.h stdio.h math.h time.h sys/select.h)


AC_HEADER_SYS_WAIT
AC_HEADER_DIRENT
AC_HEADER_MAJOR
AC_HEADER_STAT
AC_HEADER_STDC


# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_CHECK_SIZEOF(short)
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(long)


AC_CHECK_FUNCS([clock_gettime])

AC_CHECK_LIB([wiringPi], [pinMode])

#AC_CHECK_LIB([share], [get_libshare_version], , 
#  AC_MSG_ERROR("libshare (runtime library) is required")
#)




AC_OUTPUT(Makefile src/core/Makefile)


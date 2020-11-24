
##
#  Copyright 2020 Brian Burrell
#
#  This file is part of libhtm.
#  (https://github.com/neonatura/zio)
#
#  libhtm is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 2 of the License, or
#  (at your option) any later version.
#
#  libhtm is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with libhtm.  If not, see <http://www.gnu.org/licenses/>.
##

test_zpu_headers = \
	CuTest.h \
	test_libzpu.h

test_zpu_sources = \
	CuTest.c \
	test_libzpu.c \
	test_libzpu_bios.c \
	test_libzpu_math.c \
	test_libzpu_vaddr.c

test_zpu_SOURCES = $(test_zpu_sources) $(test_zpu_headers)

test_zpu_CFLAGS = \
	-DHAVE_CONFIG_H -DRUN_TEST \
	-I$(top_srcdir)/src/libzsys \
  -I$(top_srcdir)/src/libquat \
  -I$(top_srcdir)/src/libzdb \
  -I$(top_srcdir)/src/libzpu

test_zpu_LDADD = \
  $(top_builddir)/src/libzpu/libzpu.a \
  $(top_builddir)/src/libzdb/libzdb.a \
  $(top_builddir)/src/libquat/libquat.a \
  $(top_builddir)/src/libzsys/libzsys.a

test_zpu_LDFLAGS = -lm -lpthread 

noinst_PROGRAMS = test_zpu

TESTS = test_zpu


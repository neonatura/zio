
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

test_quat_headers = \
	CuTest.h \
	test_libquat.h

test_quat_sources = \
	CuTest.c \
	test_libquat.c \
	test_libquat_math.c

test_quat_SOURCES = $(test_quat_sources) $(test_quat_headers)

test_quat_CFLAGS = \
	-DHAVE_CONFIG_H -DRUN_TEST \
	-I$(top_srcdir)/src/libzsys \
  -I$(top_srcdir)/src/libquat

test_quat_LDADD = \
  $(top_builddir)/src/libquat/libquat.a \
  $(top_builddir)/src/libzsys/libzsys.a

test_quat_LDFLAGS = -lm -lpthread 

noinst_PROGRAMS = test_quat

TESTS = test_quat



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

test_htm_headers = \
	CuTest.h \
	test_libhtm.h

test_htm_sources = \
	CuTest.c \
	test_libhtm.c \
	test_libhtm_celerity.c \
	test_libhtm_position.c \
	test_libhtm_zpu.c

#	test_lang.h
#test_lang.c

test_htm_SOURCES = $(test_htm_sources) $(test_htm_headers)

test_htm_CFLAGS = \
	-DHAVE_CONFIG_H -DRUN_TEST \
	-I$(top_srcdir)/src

test_htm_LDADD = $(top_builddir)/src/libhtm.a
test_htm_LDFLAGS = -lm -lpthread 

noinst_PROGRAMS = test_htm

TESTS = test_htm


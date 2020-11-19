
quat_headers = \
	$(srcdir)/quat.h

quat_sources = \
	$(srcdir)/quat.c \
	$(srcdir)/quat_alloc.c \
	$(srcdir)/quat_decode.c \
	$(srcdir)/quat_encode.c \
	$(srcdir)/quat_get.c \
	$(srcdir)/quat_op.c \
	$(srcdir)/quat_op.h \
	$(srcdir)/quat_prec.c \
	$(srcdir)/quat_set.c \
	$(srcdir)/quat_string.c


noinst_LIBRARIES = libquat.a
libquat_a_SOURCES = $(quat_sources) $(quat_headers)

libquat_a_CFLAGS = \
	-DHAVE_CONFIG_H -I$(top_builddir) \
	-I$(top_srcdir)/src/libzsys


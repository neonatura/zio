
zsys_headers = \
	$(srcdir)/zsys_adler64.h \
	$(srcdir)/zsys_buff.h \
	$(srcdir)/zsys_error.h \
	$(srcdir)/zsys_font.h \
	$(srcdir)/zsys_laplace.h \
	$(srcdir)/zsys_log.h \
	$(srcdir)/zsys_map.h \
	$(srcdir)/zsys_path.h \
	$(srcdir)/zsys_string.h \
	$(srcdir)/zsys_time.h

zsys_sources = \
	$(srcdir)/zsys_adler64.c \
	$(srcdir)/zsys_buff.c \
	$(srcdir)/zsys_error.c \
	$(srcdir)/zsys_font.c \
	$(srcdir)/zsys_laplace.cpp \
	$(srcdir)/zsys_log.c \
	$(srcdir)/zsys_map.c \
	$(srcdir)/zsys_path.c \
	$(srcdir)/zsys_string.c \
	$(srcdir)/zsys_time.c

noinst_LIBRARIES = libzsys.a
libzsys_a_SOURCES = $(zsys_sources) $(zsys_headers)

libzsys_a_CFLAGS = \
	-DHAVE_CONFIG_H -I$(top_builddir)


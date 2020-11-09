
con_sources = \
	$(srcdir)/con_main.c \
	$(srcdir)/con_error.c

con_headers = \
	$(srcdir)/con_main.h \
	$(srcdir)/con_error.h \
	$(srcdir)/con.h

bin_PROGRAMS = zio
zio_SOURCES = $(con_sources) $(con_headers)

zio_CFLAGS = \
	-DHAVE_CONFIG_H -I$(top_srcdir)/src/libhtm/src -I$(top_srcdir)/src/libzio

zio_LDADD = \
	$(top_builddir)/$(LIBHTM) \
	$(top_builddir)/src/libzio/libzio.a \
	-lm


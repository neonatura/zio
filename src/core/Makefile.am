
core_sources = \
	$(srcdir)/core_sexe.c \
	$(srcdir)/core_main.c \
	$(srcdir)/core_error.c

core_headers = \
	$(srcdir)/core_sexe.h \
	$(srcdir)/core_main.h \
	$(srcdir)/core_error.h \
	$(srcdir)/core.h

bin_PROGRAMS = zion
zion_SOURCES = $(core_sources) $(core_headers)

zion_CFLAGS = \
	-DHAVE_CONFIG_H -I$(top_srcdir)/src/libhtm/src -I$(top_srcdir)/src/libzio

zion_LDADD = \
	$(top_builddir)/$(LIBHTM) \
	$(top_builddir)/src/libzio/libzio.a \
	-lshare_sexe -lm


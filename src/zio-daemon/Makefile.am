
ziod_sources = \
	$(srcdir)/ziod_main.c \
	$(srcdir)/ziod_error.c \
	$(srcdir)/ziod_log.c \
	$(srcdir)/ziod_opt.c

ziod_headers = \
	$(srcdir)/ziod_main.h \
	$(srcdir)/ziod_error.h \
	$(srcdir)/ziod_log.h \
	$(srcdir)/ziod_opt.h \
	$(srcdir)/ziod.h

bin_PROGRAMS = ziod
ziod_SOURCES = $(ziod_sources) $(ziod_headers)

ziod_CFLAGS = \
	-DHAVE_CONFIG_H -I$(top_srcdir)/src/libhtm/src -I$(top_srcdir)/src/libzio

ziod_LDADD = \
	$(top_builddir)/$(LIBHTM) \
	$(top_builddir)/src/libzio/libzio.a \
	-lm
if LIBPTHREAD
ziod_LDADD += -lpthread
endif
if CLOCK_GETTIME
ziod_LDADD += -lrt
endif

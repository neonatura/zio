
ziod_sources = \
	$(srcdir)/ziod_main.c \
	$(srcdir)/ziod_opt.c \
	$(srcdir)/ziod_therm.c

ziod_headers = \
	$(srcdir)/ziod_main.h \
	$(srcdir)/ziod_opt.h \
	$(srcdir)/ziod_therm.h \
	$(srcdir)/ziod.h

sbin_PROGRAMS = ziod
ziod_SOURCES = $(ziod_sources) $(ziod_headers)

ziod_CFLAGS = \
	-DHAVE_CONFIG_H \
	-I$(top_srcdir)/src/libzsys \
	-I$(top_srcdir)/src/libquat \
	-I$(top_srcdir)/src/libzdb \
	-I$(top_srcdir)/src/libzpu \
	-I$(top_srcdir)/src/libhtm \
	-I$(top_srcdir)/src/libzio

ziod_LDADD = \
	$(top_builddir)/src/libzio/libzio.a \
	$(top_builddir)/$(LIBHTM) \
	$(top_builddir)/src/libzpu/libzpu.a \
	$(top_builddir)/src/libzdb/libzdb.a \
	$(top_builddir)/src/libquat/libquat.a \
	$(top_builddir)/src/libzsys/libzsys.a \
	-lm
if LIBPTHREAD
ziod_LDADD += -lpthread
endif
if CLOCK_GETTIME
ziod_LDADD += -lrt
endif

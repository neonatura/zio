
zdb_headers = \
	$(srcdir)/zdb.h \
	$(srcdir)/zdb_arch.h \
	$(srcdir)/zdb_block.h \
	$(srcdir)/zdb_fmap.h \
	$(srcdir)/zdb_index.h \
	$(srcdir)/zdb_mutex.h \
	$(srcdir)/zdb_table.h

zdb_sources = \
	$(srcdir)/zdb_arch.c \
	$(srcdir)/zdb_block.c \
	$(srcdir)/zdb_fmap.c \
	$(srcdir)/zdb_index.c \
	$(srcdir)/zdb_mutex.c \
	$(srcdir)/zdb_table.c

noinst_LIBRARIES = libzdb.a
libzdb_a_SOURCES = $(zdb_sources) $(zdb_headers)

libzdb_a_CFLAGS = \
	-DHAVE_CONFIG_H -I$(top_builddir) \
	-I$(top_srcdir)/src/libzsys


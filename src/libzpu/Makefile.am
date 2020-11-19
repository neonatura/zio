
zpu_headers = \
	$(srcdir)/zpu.h \
	$(srcdir)/zpu_exec.h \
	$(srcdir)/zpu_init.h \
	$(srcdir)/zpu_vaddr.h \
	$(srcdir)/zpu_vaddr_rom.h \
	$(srcdir)/zpu_opcode.h \
	$(srcdir)/zpu_reg.h

zpu_sources = \
	$(srcdir)/zpu_exec.c \
	$(srcdir)/zpu_init.c \
	$(srcdir)/zpu_vaddr.c \
	$(srcdir)/zpu_vaddr_rom.c \
	$(srcdir)/zpu_opcode.c \
	$(srcdir)/zpu_reg.c

noinst_LIBRARIES = libzpu.a
libzpu_a_SOURCES = $(zpu_sources) $(zpu_headers)

libzpu_a_CFLAGS = \
	-DHAVE_CONFIG_H -I$(top_builddir) \
	-I$(top_srcdir)/src/libzsys \
	-I$(top_srcdir)/src/libquat \
	-I$(top_srcdir)/src/libzdb


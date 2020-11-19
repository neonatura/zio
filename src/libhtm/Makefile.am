
htm_headers = \
	$(srcdir)/htm_brane.h \
	$(srcdir)/htm.h \
	$(srcdir)/htm_cache.h \
	$(srcdir)/htm_celerity.h \
	$(srcdir)/htm_cell.h \
	$(srcdir)/htm_chord.h \
	$(srcdir)/htm_control.h \
	$(srcdir)/htm_entity.h \
	$(srcdir)/htm_exec.h \
	$(srcdir)/htm_exec_work.h \
	$(srcdir)/htm_emo.h \
	$(srcdir)/htm_layer.h \
	$(srcdir)/htm_map.h \
	$(srcdir)/htm_mem.h \
	$(srcdir)/htm_motor.h \
	$(srcdir)/htm_neo.h \
	$(srcdir)/htm_path.h \
	$(srcdir)/htm_position.h \
	$(srcdir)/htm_sense.h \
	$(srcdir)/htm_tree.h \
	$(srcdir)/op/htm_op.h

htm_op_sources = \
	$(srcdir)/op/htm_op_feedback.c \
	$(srcdir)/op/htm_op_pool.c \
	$(srcdir)/op/htm_op_reset.c \
	$(srcdir)/op/htm_op_seq.c

htm_sources = \
	$(srcdir)/htm_brane.c \
	$(srcdir)/htm_cache.c \
	$(srcdir)/htm_celerity.c \
	$(srcdir)/htm_cell.c \
	$(srcdir)/htm_chord.c \
	$(srcdir)/htm_control.c \
	$(srcdir)/htm_entity.c \
	$(srcdir)/htm_exec.c \
	$(srcdir)/htm_exec_work.c \
	$(srcdir)/htm_emo.c \
	$(srcdir)/htm_layer.c \
	$(srcdir)/htm_mem.c \
	$(srcdir)/htm_motor.c \
	$(srcdir)/htm_neo.c \
	$(srcdir)/htm_path.c \
	$(srcdir)/htm_position.c \
	$(srcdir)/htm_sense.c \
	$(srcdir)/htm_tree.c \
	$(htm_op_sources)

noinst_LIBRARIES = libhtm.a
libhtm_a_SOURCES = $(htm_sources) $(htm_headers)

libhtm_a_CFLAGS = \
	-DHAVE_CONFIG_H -I$(top_builddir) \
	-I$(top_srcdir)/src/libzsys \
	-I$(top_srcdir)/src/libquat \
	-I$(top_srcdir)/src/libzdb \
	-I$(top_srcdir)/src/libzpu



htm_headers = \
	$(srcdir)/htm_adler64.h \
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
	$(srcdir)/htm_tree.h \
	$(srcdir)/htm_zpu.h \
	$(srcdir)/util/htm_buff.h \
	$(srcdir)/util/htm_time.h \
	$(srcdir)/lang/lang_char.h \
	$(srcdir)/op/htm_op.h \
	$(srcdir)/quat/quat.h

htm_lang_sources = \
	$(srcdir)/lang/lang_char.c \
	$(srcdir)/lang/lang_char_b.c \
	$(srcdir)/lang/lang_char_c.c \
	$(srcdir)/lang/lang_char_d.c \
	$(srcdir)/lang/lang_char_e.c

htm_op_sources = \
	$(srcdir)/op/htm_op_feedback.c \
	$(srcdir)/op/htm_op_pool.c \
	$(srcdir)/op/htm_op_reset.c \
	$(srcdir)/op/htm_op_seq.c

htm_quat_sources = \
	$(srcdir)/quat/quat.c \
	$(srcdir)/quat/quat_alloc.c \
	$(srcdir)/quat/quat_decode.c \
	$(srcdir)/quat/quat_encode.c \
	$(srcdir)/quat/quat_get.c \
	$(srcdir)/quat/quat_op.c \
	$(srcdir)/quat/quat_op.h \
	$(srcdir)/quat/quat_prec.c \
	$(srcdir)/quat/quat_set.c \
	$(srcdir)/quat/quat_string.c


htm_sources = \
	$(srcdir)/htm_adler64.c \
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
	$(srcdir)/htm_laplace.cpp \
	$(srcdir)/htm_layer.c \
	$(srcdir)/htm_map.c \
	$(srcdir)/htm_mem.c \
	$(srcdir)/htm_motor.c \
	$(srcdir)/htm_neo.c \
	$(srcdir)/htm_path.c \
	$(srcdir)/htm_position.c \
	$(srcdir)/htm_tree.c \
	$(srcdir)/htm_zpu.c \
	$(srcdir)/util/htm_buff.c \
	$(srcdir)/util/htm_time.c \
	$(htm_lang_sources) \
	$(htm_op_sources) \
	$(htm_quat_sources)

lib_LIBRARIES = libhtm.a
libhtm_a_SOURCES = $(htm_sources) $(htm_headers)

libhtm_a_CFLAGS = \
	-DHAVE_CONFIG_H -I$(top_builddir)
#if CLOCK_GETTIME
#libhtm_a_LIBADD = \
#	-lrt
#endif

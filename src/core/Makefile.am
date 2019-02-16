

zio_sources = \
	$(srcdir)/zio_cycle.c \
	$(srcdir)/zio_error.c \
	$(srcdir)/zio_debug.c \
	$(srcdir)/zio_dev.c \
	$(srcdir)/zio_gps.c \
	$(srcdir)/zio_itemp.c \
	$(srcdir)/zio_pin.c \
	$(srcdir)/zio_time.c \
	$(srcdir)/dev/zio_therm.c

#	$(srcdir)/zio_dht.c 
#	$(srcdir)/zio_gyro.c
#	$(srcdir)/zio_speaker.c
# $(srcdir)/zio_i2c.c

zio_headers = \
	$(srcdir)/zio.h \
	$(srcdir)/zio_audio.h \
	$(srcdir)/zio_cycle.h \
	$(srcdir)/zio_debug.h \ 
	$(srcdir)/zio_dev.h \ 
	$(srcdir)/zio_error.h \
	$(srcdir)/zio_gps.h \
	$(srcdir)/zio_itemp.h \
	$(srcdir)/zio_pin.h \
	$(srcdir)/zio_time.h \
	$(srcdir)/dev/zio_therm.h

#$(srcdir)/zio_dht.h
#	$(srcdir)/zio_gyro.h 
#	$(srcdir)/zio_speaker.h
#	$(srcdir)/zio_i2c.h 


lib_LIBRARIES = libzio.a
libzio_a_SOURCES = $(zio_sources) $(zio_headers)

libzio_a_CFLAGS = \
	-DHAVE_CONFIG_H -DZIO=1 -DPI 
#-DUSE_WIRINGPI





zio_sources = \
	$(srcdir)/zio_audio.c \
	$(srcdir)/zio_ctl.c \
	$(srcdir)/zio_cycle.c \
	$(srcdir)/zio_error.c \
	$(srcdir)/zio_geo.c \
	$(srcdir)/zio_dev.c \
	$(srcdir)/zio_mod.c \
	$(srcdir)/zio_obj.c \
	$(srcdir)/zio_pin.c \
	$(srcdir)/zio_time.c \
	$(srcdir)/dev/dev_air.c \
	$(srcdir)/dev/dev_audio.c \
	$(srcdir)/dev/dev_debug.c \
	$(srcdir)/dev/dev_dht.c \
	$(srcdir)/dev/dev_dummy_temp.c \
	$(srcdir)/dev/dev_geo.c \
	$(srcdir)/dev/dev_gps.c \
	$(srcdir)/dev/dev_itemp.c \
	$(srcdir)/dev/dev_gtime.c \
	$(srcdir)/dev/dev_itime.c \
	$(srcdir)/dev/dev_lcd_ssd1306.c \
	$(srcdir)/dev/dev_lcd_twi1602.c \
	$(srcdir)/dev/dev_led.c \
	$(srcdir)/dev/dev_ldr.c \
	$(srcdir)/dev/dev_log.c \
	$(srcdir)/dev/dev_lux.c \
	$(srcdir)/dev/dev_mood.c \
	$(srcdir)/dev/dev_motion.c \
	$(srcdir)/dev/dev_rtc.c \
	$(srcdir)/dev/dev_rtemp.c \
	$(srcdir)/dev/dev_sc16is.c \
	$(srcdir)/dev/dev_sgp30.c \
	$(srcdir)/dev/dev_speaker.c \
	$(srcdir)/dev/dev_time.c \
	$(srcdir)/dev/dev_therm.c \
	$(srcdir)/dev/dev_thermistor.c

zio_headers = \
	$(srcdir)/zio.h \
	$(srcdir)/zio_audio.h \
	$(srcdir)/zio_ctl.h \
	$(srcdir)/zio_cycle.h \
	$(srcdir)/zio_dev.h \
	$(srcdir)/zio_dummy_temp.h \
	$(srcdir)/zio_error.h \
	$(srcdir)/zio_geo.h \
	$(srcdir)/zio_mod.h \
	$(srcdir)/zio_obj.h \
	$(srcdir)/zio_pin.h \
	$(srcdir)/zio_time.h \
	$(srcdir)/dev/dev_air.h \
	$(srcdir)/dev/dev_audio.h \
	$(srcdir)/dev/dev_debug.h \
	$(srcdir)/dev/dev_dht.h \
	$(srcdir)/dev/dev_dummy_temp.h \
	$(srcdir)/dev/dev_geo.h \
	$(srcdir)/dev/dev_gps.h \
	$(srcdir)/dev/dev_itemp.h \
	$(srcdir)/dev/dev_gtime.h \
	$(srcdir)/dev/dev_itime.h \
	$(srcdir)/dev/dev_lcd_ssd1306.h \
	$(srcdir)/dev/dev_lcd_twi1602.h \
	$(srcdir)/dev/dev_led.h \
	$(srcdir)/dev/dev_ldr.h \
	$(srcdir)/dev/dev_log.h \
	$(srcdir)/dev/dev_lux.h \
	$(srcdir)/dev/dev_mood.h \
	$(srcdir)/dev/dev_motion.h \
	$(srcdir)/dev/dev_rtc.h \
	$(srcdir)/dev/dev_rtemp.h \
	$(srcdir)/dev/dev_sc16is.h \
	$(srcdir)/dev/dev_sgp30.h \
	$(srcdir)/dev/dev_speaker.h \
	$(srcdir)/dev/dev_time.h \
	$(srcdir)/dev/dev_therm.h \
	$(srcdir)/dev/dev_thermistor.h

lib_LIBRARIES = libzio.a
libzio_a_SOURCES = $(zio_sources) $(zio_headers)

libzio_a_CFLAGS = \
	-DHAVE_CONFIG_H -DZIO=1 -DPI 


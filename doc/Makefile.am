AM_CFLAGS = -DDATADIR=\"$(datadir)\"
htmldir = html 
dist_docdir = $(datadir)/@PACKAGE@


if HAVE_HELP2MAN
dist_man_MANS = \
	man/man1/ziod.1

man/man1/ziod.1: helpfile.stamp

helpfile.stamp:
	mkdir -p man/man1
	$(HELP2MAN) --output=man/man1/ziod.1 --name='zio daemon' $(top_builddir)/src/zio-daemon/ziod
	echo Timestamp > helpfile.stamp
endif


if HAVE_DOXYGEN
dist_doc_DATA = zio_html.tar.gz

$(htmldir): doxyfile.stamp

doxyfile.stamp:
	mkdir -p html man
	$(DOXYGEN) doxygen.conf
	echo Timestamp > doxyfile.stamp

zio_html.tar.gz: doxyfile.stamp
	rm -f zio_html.tar.gz
	tar -cpf zio_html.tar $(htmldir)
	gzip zio_html.tar
	rm -f zio_html.tar
endif

EXTRA_DIST = $(htmldir)

clean-local:
	rm -f doxyfile.stamp helpfile.stamp
	rm -rf $(builddir)/man
	rm -rf $(builddir)/html

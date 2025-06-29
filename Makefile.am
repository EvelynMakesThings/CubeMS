SUBDIRS = src
MAINTAINERCLEANFILES = \
    aclocal.m4 configure config.h.in config.log config.status \
    Makefile.in */Makefile.in \
    compile config.guess config.sub install-sh missing depcomp \
    ltmain.sh stamp-h1 \
    autom4te.cache
debug:
	$(MAKE) CFLAGS="$(CFLAGS) -g -O0 -DDEBUG"
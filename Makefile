# Allegro 4.2.2 for DOS example
# MIT license

CC        = $(DJGPP_CC)
VENDOR    = vendor
CFLAGS    = -DHAVE_STDBOOL_H=1 -fgnu89-inline -Ivendor/allegro-4.2.2-xc/include
LDFLAGS   = -Lvendor/allegro-4.2.2-xc/lib/djgpp -lalleg

BIN       = main.exe
SRCDIR    = src
OBJDIR    = obj
DISTDIR   = dist
STATICDIR = static

# Static files, e.g. the readme.txt file, that get copied straight to
# the dist directory.
STATIC    = $(shell find $(STATICDIR) -name "DATA.DAT" -not -name ".*" 2> /dev/null)
STATICDEST= $(subst $(STATICDIR),$(DISTDIR),$(STATIC))

# All source files (*.c) and their corresponding object files.
SRC       = $(shell find $(SRCDIR) -name "*.c" 2> /dev/null)
OBJS      = $(SRC:%.c=%.o)

.PHONY: clean static
default: all

# Check whether DJGPP is available.
ifndef DJGPP_CC
  $(error To compile, you'll need to set the DJGPP_CC environment variable to a DJGPP GCC binary, e.g. /usr/local/djgpp/bin/i586-pc-msdosdjgpp-gcc)
endif

${DISTDIR}:
	mkdir -p ${DISTDIR}

%.o: %.c
	${CC} -c -o $@ $? ${CFLAGS} -O3

${DISTDIR}/${BIN}: ${OBJS}
	${CC} -o ${DISTDIR}/${BIN} $+ ${LDFLAGS} -O3
	${CC} -o static/setup.exe setup/setup.c ${CFLAGS} -O3 ${LDFLAGS}


${STATICDEST}:
	cd $(STATICDIR) && sh convert.sh	
	@mkdir -p $(shell dirname $@)
	cp $(subst $(DISTDIR),$(STATICDIR),$@) $@
	cp static/cwsdpmi.exe ${DISTDIR}
	cp static/msdos.pcx ${DISTDIR}
	cp static/setup.* ${DISTDIR}

all: ${DISTDIR} ${DISTDIR}/${BIN} ${STATICDEST}

static: ${STATICDEST}

clean:
	rm -rf ${DISTDIR}
	rm -f ${OBJS}
	touch ${STATICDIR}/data.dat

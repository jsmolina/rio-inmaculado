# Allegro 4.2.2 for DOS example
# MIT license

CC        = $(DJGPP_CC)
VENDOR    = vendor
CFLAGS    = -DHAVE_STDBOOL_H=1 -fgnu89-inline -march=i386 -mno-80387 -mno-fp-ret-in-387 -Ivendor/allegro-4.2.2-xc/include
# -lemu links DJGPP's x87 emulator: Allegro/libc contain FPU instructions, which crash 386/486SX without it
LDFLAGS   = -Lvendor/allegro-4.2.2-xc/lib/djgpp -lalleg -lemu

BIN       = main.exe
SRCDIR    = src
OBJDIR    = obj
DISTDIR   = dist
STATICDIR = static

# Game datafile, packed by static/build.sh (docker-compose runs it first), copied to dist.
STATIC    = $(STATICDIR)/datos.dat
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
	${CC} -c -o $@ $< ${CFLAGS} -O3

# rebuild when a header changes (statics.h indices change whenever datos.dat is repacked)
${OBJS}: $(wildcard $(SRCDIR)/*.h)

${DISTDIR}/${BIN}: ${OBJS}
	${CC} -o ${DISTDIR}/${BIN} $+ ${LDFLAGS} -O3
	${CC} -o static/setup.exe setup/setup.c ${CFLAGS} -O3 ${LDFLAGS}


# copied again whenever static/build.sh repacks the datafile
${STATICDEST}: ${STATIC}
	@mkdir -p $(shell dirname $@)
	cp $(subst $(DISTDIR),$(STATICDIR),$@) $@
	cp static/cwsdpmi.exe ${DISTDIR}
	cp static/setup.* ${DISTDIR}
	cp static/msdos.bmp ${DISTDIR}
	cp static/shareware.txt ${DISTDIR}/SHARE.TXT

all: ${DISTDIR} ${DISTDIR}/${BIN} ${STATICDEST}

static: ${STATICDEST}

clean:
	rm -f ${DISTDIR}/*
	rm -f ${OBJS}

PREFIX = /opt/local

CC = cc
FLAGS = -Wall -pedantic -Os -std=c99 -D_POSIX_C_SOURCE=200809L
PROGS.C = batlog.c PStime.c execbg.c
PROGS = ${PROGS.C:.c=}
FILES = ${PROGS} brightness colors colors-all epsunite\
        pdfsepunite replaceindir svg2pdf texclear\
        usb volume showgit

define NL


endef

all: ${PROGS} ${FILES}

install: all
	mkdir -p ${PREFIX}/bin
	cp -f ${FILES} ${PREFIX}/bin
	for file in ${FILES}; do chmod 755 ${PREFIX}/bin/$$file; done

uninstall:
	  for file in ${FILES}; do rm -f ${PREFIX}/bin/$$file; done

clean:
	rm -f ${PROGS}

%: %.c
	${CC} ${FLAGS} -o $@ $<

%:

.PHONY: all install uninstall clean

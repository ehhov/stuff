PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

CC = cc
CFLAGS = -std=c99 -Wall -pedantic -Os -D_POSIX_C_SOURCE=200809L
LIBS = -lpng

all: halfmir

halfmir: halfmir.c
	${CC} ${CFLAGS} ${LIBS} $< -o $@

clean:
	rm -f halfmir

install: all
	mkdir -p ${PREFIX}/bin
	cp -f halfmir ${PREFIX}/bin
	chmod 755 ${PREFIX}/bin/halfmir
	mkdir -p ${MANPREFIX}/man1
	cp -f halfmir.1 ${MANPREFIX}/man1
	chmod 644 ${MANPREFIX}/man1/halfmir.1

uninstall:
	rm -f ${PREFIX}/bin/halfmir
	rm -f ${MANPREFIX}/man1/halfmir.1

.PHONY: all clean install uninstall

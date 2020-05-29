LIBS=-lX11 -lpthread -lpulse
FLAGS=-std=c99 -Wall -pedantic -Os -D_DEFAULT_SOURCE -D_POSIX_C_SOURCE=2
FUNCS=battery.c clock.c layout.c network.c volume.c
OBJS=${FUNCS:.c=.o}

all: dwms tint2s

%.o: %.c structs.h
	gcc -c ${FLAGS} -o $@ $<

dwms: dwms.o ${OBJS} dwms.h tuning.h
	gcc ${LIBS} ${OBJS} $< -o $@

tint2s: tint2s.o ${OBJS} tint2s.h tuning.h
	gcc ${LIBS} ${OBJS} $< -o $@

clear:
	rm ${OBJS} dwms.o tint2s.o

.PHONY: clear all

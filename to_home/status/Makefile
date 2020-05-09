LIBS=-lX11 -lpthread -lpulse
FLAGS=-Wall -O2
FUNCS=battery.c clock.c layout.c network.c volume.c
OBJ=${FUNCS:.c=.o}

all: dwms tint2s

${OBJ}: 
dwms.o:
tint2s.o:
.c.o:
	gcc -c ${FLAGS} $<

dwms: ${OBJ} dwms.o
	gcc ${LIBS} ${OBJ} dwms.o -o $@

tint2s: ${OBJ} tint2s.o
	gcc ${LIBS} ${OBJ} tint2s.o -o $@

clear:
	rm ${OBJ} dwms.o tint2s.o

.PHONY: clear all

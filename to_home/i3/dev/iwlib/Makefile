all:
	gcc -Os -W -Wall -Wstrict-prototypes -Wmissing-prototypes -Wshadow -Wpointer-arith -Wcast-qual -Winline -I. -MMD     -fPIC -c -o iwlib.so iwlib.c
	gcc -shared -o libiw.so.29 -Wl,-soname,libiw.so.29  -lm -lc iwlib.so

FILES.C = brightness_levels.c battery_log.c bash_time.c
FILES.C.OUT = $(FILES.C:.c=)

all: $(FILES.C.OUT)

%: %.c
	gcc -Wall -pedantic -Os -std=c99 -o $@ $<

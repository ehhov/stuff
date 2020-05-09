FILES = brightness_levels battery_log bash_time

.PHONY: all
all:
	$(foreach file, $(FILES), gcc $(file).c -o $(file);)

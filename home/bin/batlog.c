#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define BAT_PATH "/sys/class/power_supply/BAT0/"

int interval_sec = 60;
int stop_after_min = 60;

int
main(int argc, char *argv[])
{
	FILE *file;
	int now, full, current;
	time_t t, started;
	double percent, hours_remain;
	const char *cmd;

	for (cmd = argv[0], argv++; argv[0] && argv[0][0] == '-' && argv[0][1]; argc--, argv++) {
		char opt = argv[0][1];
		switch (opt) {
		case 't':
			if (argv[0][2]) {
				argv[0] += 2;
				stop_after_min = atoi(argv[0]);
			} else {
				stop_after_min = atoi(argv[1]);
				argc--; argv++;
			}
			break;
		case 'i':
			if (argv[0][2]) {
				argv[0] += 2;
				interval_sec = atoi(argv[0]);
			} else {
				interval_sec = atoi(argv[1]);
				argc--; argv++;
			}
			break;
		case 'h':
			printf("%s -i intreval_sec -t stop_after_min\n", cmd);
			return 0;
			break;
		default:
			break;
		}
	}

	time(&started);
	do {
		file = fopen(BAT_PATH"charge_now", "r");
		if (file == NULL) {
			fprintf(stderr, "file does not exist (%s).\n", BAT_PATH"charge_now");
			return 1;
		}
		fscanf(file, "%d", &now);
		fclose(file);
		
		file = fopen(BAT_PATH"charge_full", "r");
		if (file == NULL) {
			fprintf(stderr, "file does not exist (%s).\n", BAT_PATH"charge_full");
			return 1;
		}
		fscanf(file, "%d", &full);
		fclose(file);
	
		file = fopen(BAT_PATH"current_now", "r");
		if (file == NULL) {
			fprintf(stderr, "file does not exist (%s).\n", BAT_PATH"current_now");
			return 1;
		}
		fscanf(file, "%d", &current);
		fclose(file);
		if (current == 0) current = 1;
	
		percent = now * 100.0 / full;
		hours_remain = now * 1.0 / current;
		time(&t);
		printf("%.2lf %.2lf %.3lf\n", (t - started)*1.0/60 , percent, hours_remain);
		fflush(stdout);
		sleep(interval_sec);
	} while (t - started < stop_after_min*60);
	
	return 0;
}

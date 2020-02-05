#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define BAT_PATH "/sys/class/power_supply/BAT0/"

int main () {
	FILE *file;
	int now, full, current;
	time_t t;
	double percent, hours_remain; 

	while (1) {
		file=fopen(BAT_PATH"charge_now","r");
		fscanf(file,"%d",&now);
		fclose(file);
		
		file=fopen(BAT_PATH"charge_full","r");
		fscanf(file,"%d",&full);
		fclose(file);
	
		file=fopen(BAT_PATH"current_now","r");
		fscanf(file,"%d",&current);
		fclose(file);
	
		percent = now*100.0/full;
		hours_remain = now*1.0/current;
		time(&t);
		printf("%ld %lf %lf\n", t, percent, hours_remain);
		fflush(stdout);
		sleep(15);
	}
	
	return 0;
}

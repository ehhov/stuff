#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define BAT_PATH "/sys/class/power_supply/BAT0/"

double interval_minutes = 1;
double stop_after_hours = 1;

int main() {
	FILE *file;
	int now, full, current;
	time_t t, started;
	double percent, hours_remain; 

	time(&started);
	do {
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
		printf("%.2lf %.2lf %.3lf\n", (t - started)*1.0/60 , percent, hours_remain);
		fflush(stdout);
		sleep((int) interval_minutes*60);
	} while (t - started < stop_after_hours*3600);
	
	return 0;
}

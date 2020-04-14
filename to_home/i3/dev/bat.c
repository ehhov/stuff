#include <stdio.h>
#include <time.h>

#define FULL_TEXT "\{\"full_text\": \""

int main (int argc, char ** argv) {
	FILE *file;
	double battery_now, battery_time, full;
 
	int i=0;
	printf("\{\"version\":1\}\n[\n");
	while (1) {
	file=fopen("/sys/class/power_supply/BAT0/charge_now","r");
  fscanf(file,"%lf",&battery_now);
  fclose(file);
 
  file=fopen("/sys/class/power_supply/BAT0/charge_full","r");
  fscanf(file,"%lf",&full);
  fclose(file);

  file=fopen("/sys/class/power_supply/BAT0/current_now","r");
  fscanf(file,"%lf",&battery_time);
  fclose(file);
 
	battery_time=battery_now/battery_time;
	battery_now=battery_now/full*100;

	/*printf("%.2f%% %d:%02d\n", battery_now, (int)battery_time, (int)(60*(battery_time-(int)battery_time)));
	if (battery_now>75)
    printf("#00ff00");
   else if (battery_now>40)
    printf("#aadd00");
   else if (battery_now>15)
    printf("#ffcc00");
   else printf("#ff5500");
	printf("\n");*/
	

		printf("[\n");
		printf(FULL_TEXT "%.2f%% %d:%02d", battery_now, (int)battery_time, (int)(60*(battery_time-(int)battery_time)));
		printf("\", \"color\": \"#999999\"\},\n"FULL_TEXT"%d\", \"color\": \"#ffffff\"\} \n],\n",i++);
		fflush(stdout);
		fflush(stdout);
		fflush(stdout);
		sleep(1);
	}


	return 0;
}

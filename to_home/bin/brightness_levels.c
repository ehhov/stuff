#include <stdio.h>

#define BRIGHTNESS_PATH "/sys/class/backlight/intel_backlight/"

int main () {
	double percent;
	FILE* file;
  int max, actual;

  file=fopen(BRIGHTNESS_PATH"max_brightness","r");
  fscanf(file,"%d",&max);
  fclose(file);
      
  file=fopen(BRIGHTNESS_PATH"actual_brightness","r");
  fscanf(file,"%d",&actual);
  fclose(file);

  percent=100.0*actual/max;

	if (percent>60)	printf("0\n");
	else if (percent>15) printf("1\n");
	else if (percent>5) printf("2\n");
	else printf("3\n");
	
	
	return 0;
}

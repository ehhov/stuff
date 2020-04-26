#include <stdio.h>

#define BRIGHTNESS_PATH "/sys/class/backlight/intel_backlight/"

int main () {
	FILE* file;
	double percent;
  int max, actual;

  file=fopen(BRIGHTNESS_PATH"max_brightness","r");
  fscanf(file,"%d",&max);
  fclose(file);
      
  file=fopen(BRIGHTNESS_PATH"actual_brightness","r");
  fscanf(file,"%d",&actual);
  fclose(file);

  percent=100.0*actual/max;

	if (percent>60)	printf("%lf\n", max*.05);
	else if (percent>15) printf("%lf\n", max*.02);
	else if (percent>5) printf("%lf\n", max*.01);
	else printf("%lf\n", max*.005);
	
	return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>


#define BRIGHTNESS_PATH "/sys/class/backlight/intel_backlight/"


char scale_blocks[31][41]=
        {"░░░░░░░░░░","▒░░░░░░░░░","▓░░░░░░░░░",
         "█░░░░░░░░░","█▒░░░░░░░░","█▓░░░░░░░░",
         "██░░░░░░░░","██▒░░░░░░░","██▓░░░░░░░",
         "███░░░░░░░","███▒░░░░░░","███▓░░░░░░",
         "████░░░░░░","████▒░░░░░","████▓░░░░░",
         "█████░░░░░","█████▒░░░░","█████▓░░░░",
         "██████░░░░","██████▒░░░","██████▓░░░",
         "███████░░░","███████▒░░","███████▓░░",
         "████████░░","████████▒░","████████▓░",
         "█████████░","█████████▒","█████████▓",
         "██████████"};


int main () {
	int brightness_percent, lol;
	FILE* file;
  int max, actual;

  file=fopen(BRIGHTNESS_PATH"max_brightness","r");
  fscanf(file,"%d",&max);
  fclose(file);
        
  file=fopen(BRIGHTNESS_PATH"actual_brightness","r");
  fscanf(file,"%d",&actual);
  fclose(file);

	brightness_percent=100*actual/max;
	lol=brightness_percent/3.3;
	
	printf("Brightness: %s",scale_blocks[lol]);
	fflush(stdout);

	//sleep(5);

	return 0;
}


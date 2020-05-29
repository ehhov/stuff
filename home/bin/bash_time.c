#include <stdio.h>
#include <time.h>
#include <sys/time.h>

int main() {
	time_t t;
	struct tm now;
	char *ampm_style[2]={"am","pm"};
	int is_pm;
	int hour;
	
	time(&t);
	now=*localtime(&t);
	if(now.tm_hour>=12) { is_pm=1; hour=now.tm_hour-12; }
	else { is_pm=0; hour=now.tm_hour; }
	if (hour==0) hour=12;

	printf("%d:%02d:%02d%s\n", hour, now.tm_min, now.tm_sec, ampm_style[is_pm]);
	
	return 0;
}

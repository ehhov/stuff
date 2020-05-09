#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include <X11/Xutil.h>

#include "dwms.h"
#include "tune.h"

pthread_t status_thread, kb_thread;
/* Volume has to be global because of signal capturing */
static Volume vol; 

void
sig_usr(int signal)
{	
	if (signal == SIGUSR1) 
		vol = volume_pulse(); 
	/* else do nothing, but still interrupt sleep */
}


/* Main */

int 
main() 
{
	const int max = 1024;
	size_t pos;
	char status[max];

	Battery bat;
	Clock now;
	int kb_layout;
	Network net;

	struct timeval current; 
	struct timespec wait; 
	unsigned int count = 0;
	Display *display;
	display = XOpenDisplay(NULL);

	status_thread = pthread_self();
	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = sig_usr;
	sigaction(SIGUSR1, &action, NULL);
	sigaction(SIGUSR2, &action, NULL);

	pthread_create(&kb_thread, NULL, capture_layout, &kb_layout);
	
	net = netspeed(wlan);
	vol = volume_pulse();
	
	/* Infinite loop begins */
	while (1) 
	{
		bat = battery(batn);
		now = datetime();
		net = netspeed(wlan);
		count++;

		pos = 0;
		pos += snprintf(status+pos, max-pos, beginning);
		pos += snprintf(status+pos, max-pos, "%s:%.2lf%s%.2lf", essid(wlan), \
				(net.in>0)? net.in:0, net_downup, (net.out>0)?net.out:0);
		pos += snprintf(status+pos, max-pos, separator);		
		pos += snprintf(status+pos, max-pos, "%s:%s%ld%s", vol.is_on?"V":"M", \
				vol.percent<10?" ":"", vol.percent, vol.percent<100?"%":"");
		pos += snprintf(status+pos, max-pos, separator);		
		pos += snprintf(status+pos, max-pos, "%s", Xkb_group_text[kb_layout]);
		pos += snprintf(status+pos, max-pos, separator);		
		pos += snprintf(status+pos, max-pos, "%.2lf%%%s %02i:%02i%s", \
				bat.percent, (bat.is_chr? "+" : ""), bat.hours, bat.mins, \
				(bat.percent>10 || bat.is_chr)?"":(count%2==0?"":" LOW!") );
		pos += snprintf(status+pos, max-pos, separator);		
		pos += snprintf(status+pos, max-pos, "%s %s %d %d:%02d %s", \
				WeekDays[now.wday], Months[now.mon], now.mday, \
				now.hour, now.min, ampmstyle[now.is_pm]);
		pos += snprintf(status+pos, max-pos, ending);		
		
		XStoreName(display, DefaultRootWindow(display), status);
		XSync(display, False);

		gettimeofday(&current, NULL);
		wait.tv_sec = interval - 1 - (current.tv_sec % interval);
		wait.tv_nsec = (1e6 - current.tv_usec) * 1000;
		nanosleep(&wait, NULL);
	}
	/* Infinite loop ends */
	
	pthread_join(kb_thread, NULL);

	return 0;
}

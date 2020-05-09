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

#include "tint2s.h"
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

void 
color (const char thecolor[7]) 
{
	printf(" foreground=\"#%s\"", thecolor);
	return ;
}

void 
background (const char thecolor[7]) 
{
	printf(" background=\"#%s\"", thecolor);
	return ;
} 



/* Main */
int 
main() 
{
	Battery bat;
	Clock now;
	int kb_layout;
	Network net;

	struct timeval current; 
	struct timespec wait; 
	unsigned int count = 0;

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

		printf(SP);	color("666666");
		printf("> %s: %.2lf%s%.2lf "PS, essid(wlan), (net.in>0)? net.in:0, \
				net_downup, (net.out>0)?net.out:0);
		
		printf("%s",volume_icon[vol.icon]);
		
		printf("%s", Xkb_group_icon[kb_layout]);
		
		printf(SP);
		if (bat.percent>75)
			color("00ff00");
		else if (bat.percent>40)
			color("aadd00");
		else if (bat.percent>15)
			color("ffcc00");
		else
			color("ff5500");
		printf("> %.2lf%%%s %02i:%02i "PS, bat.percent, \
				(bat.is_chr? "+" : ""), bat.hours, bat.mins);
		
		printf(" %s %s %d %d:%02d %s ", WeekDays[now.wday], Months[now.mon], \
				now.mday, now.hour, now.min, ampmstyle[now.is_pm]);

		printf("\n");
		fflush(stdout);
		gettimeofday(&current, NULL);
		wait.tv_sec = interval - 1 - (current.tv_sec % interval);
		wait.tv_nsec = (10e5 - current.tv_usec) * 1000;
		nanosleep(&wait, NULL);
	}
	/* Infinite loop ends */
	
	pthread_join(kb_thread, NULL);

	return 0;
}

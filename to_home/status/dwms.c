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
sig_usr1(int signal)
{	vol = calc_volume_pulse(); }

void
sig_usr2(int signal)
{ } /* Does nothing, but still interrupts sleep() */


/* Main */

int 
main() 
{
	const int net_len=id_len+20, vol_len=10, kb_len=4, bat_len=20, time_len=20;
	const int status_len=net_len+vol_len+kb_len+bat_len+time_len+20;
	char net_str[net_len], vol_str[vol_len], kb_str[kb_len], bat_str[bat_len], time_str[time_len];
	char status_str[status_len];

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
  action.sa_handler = sig_usr1;
  sigaction(SIGUSR1, &action, NULL);
	memset(&action, 0, sizeof(struct sigaction));
  action.sa_handler = sig_usr2;
  sigaction(SIGUSR2, &action, NULL);

	pthread_create(&kb_thread, NULL, capture_layout, &kb_layout);
	
	bat = calc_battery(battery);
	now = calc_time();
	net = calc_net(wlan);
	vol = calc_volume_pulse();
	
	/* Infinite loop begins */
	while (1) 
	{
		bat = calc_battery(battery);
		now = calc_time();
		net = calc_net(wlan);
		count++;

		snprintf(net_str, net_len, "%s:%.2lf%s%.2lf", net.essid, \
				(net.in>0)? net.in:0, net_downup, (net.out>0)?net.out:0);
		
		snprintf(vol_str, vol_len, "%s:%s%ld%s", vol.is_on?"V":"M", \
				vol.percent<10?" ":"", vol.percent, vol.percent<100?"%":"");
		
		snprintf(kb_str, kb_len, "%s", Xkb_group_text[kb_layout]);
		
		snprintf(bat_str, bat_len, "%.2lf%%%s %02i:%02i%s", bat.percent, \
				(bat.is_chr? "+" : ""), bat.hours, bat.mins, \
				(bat.percent>10 || bat.is_chr)?"":(count%2==0?"":" LOW!") );
		
		snprintf(time_str, time_len, "%s %s %d %d:%02d %s", \
				WeekDays[now.wday], Months[now.mon], now.mday, \
				now.hour, now.min, ampmstyle[now.is_pm]);

		snprintf(status_str, status_len, BEG"%s"SEP"%s"SEP"%s"SEP"%s"SEP"%s"END, \
				net_str, vol_str, kb_str, bat_str, time_str);
		
		XStoreName(display, DefaultRootWindow(display), status_str);
		XSync(display, False);

		gettimeofday(&current, NULL);
		wait.tv_sec = interval - 1 - (current.tv_sec % interval);
		wait.tv_nsec = (10e5 - current.tv_usec) * 1000;
		nanosleep(&wait, NULL);
	}
	/* Infinite loop ends */
	
	pthread_join(kb_thread, NULL);

	return 0;
}

#include <stdio.h>
#include <stdarg.h>
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
#include "tuning.h"

/* global variables */
int done = 0;
static pthread_t status_thread;

int
refresh()
{
	return pthread_kill(status_thread, SIGUSR1);
}

int
die(const char* fmt, ...)
{
	va_list arg;
	int ret;
	va_start (arg, fmt);
	ret = vfprintf (stderr, fmt, arg);
	va_end (arg);
	done = 1;
	refresh();
	return ret;
}

static void
finish(int signal)
{
	done = 1;
}

static void
sigusr(int signal)
{	/* do nothing, but still interrupt sleep */ }

void
color(const char thecolor[7])
{
	printf(" foreground=\"#%s\"", thecolor);
}


int
main()
{
	pthread_t kb_thread = 0, vol_thread = 0;
	struct timeval current;
	struct timespec wait;
	sigset_t sigset;
	struct sigaction action;

	static Battery bat;
	static Clock now;
	static int kb_layout;
	static Network net;
	static Volume vol;

	if (sigemptyset(&sigset) ||\
			sigaddset(&sigset, SIGINT) ||\
			sigaddset(&sigset, SIGTERM) ||\
			pthread_sigmask(SIG_SETMASK, &sigset, NULL)) {
		die("failed to set signal mask for secondary threads.");
		goto loop;
	}

	status_thread = pthread_self();
	if (pthread_create(&kb_thread, NULL, capture_layout, &kb_layout) ||\
			pthread_create(&vol_thread, NULL, volume, &vol)) {
		die("failed to create layout and volume threads.");
		goto loop;
	}
	net = netspeed(wlan);
	
	if (sigemptyset(&sigset) ||\
			pthread_sigmask(SIG_SETMASK, &sigset, NULL)) {
		die("failed to set signal mask for the main thread.");
		goto loop;
	}
	memset(&action, 0, sizeof(struct sigaction));
  action.sa_handler = sigusr;
  if (sigaction(SIGUSR1, &action, NULL)) {
		die("failed to set signal handler for SIGUSR1.");
		goto loop;
	}
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = finish;
	if (sigaction(SIGINT, &action, NULL) ||\
			sigaction(SIGTERM, &action, NULL)) {
		die("failed to set signal handler for SIGINT and SIGTERM.");
		goto loop;
	}

loop:
	/* Infinite loop begins */
	while (!done)
	{
		bat = battery(batn);
		now = datetime();
		net = netspeed(wlan);

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
	
	if (kb_thread != 0 && vol_thread != 0) {
		pthread_kill(kb_thread, SIGUSR1);
		pthread_kill(vol_thread, SIGUSR1);
		pthread_join(kb_thread, NULL);
		pthread_join(vol_thread, NULL);
	}

	return 0;
}

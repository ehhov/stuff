/*
--------------------- NOTES ---------------------
the full information about power devices is located in uevent file
the paths to batteries can be found with 
  find / -type d -name power_supply
the paths are: 
  /sys/devices/LNXSYSTM:00/LNXSYBUS:00/ACPI0003:00/power_supply/AC
  /sys/devices/LNXSYSTM:00/LNXSYBUS:00/PNP0C0A:00/power_supply/BAT0

for many symbols see 
  https://jrgraphix.net/r/Unicode/
	https://emojipedia.org
useful symbols: 
  🔇🔈🔉🔊 -- emoji style speaker
       -- text style speaker
  ░▒▓█
	• ·┄
  🇺🇸 🇷🇺 
  ☀🔅🔆 
  ↓↑ ⇅ ⇵
-------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include <ifaddrs.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/wireless.h>

#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include <X11/Xutil.h>
#include <X11/XKBlib.h>

/* Just easier than variables... */
#define BAT "BAT0"
#define AC "AC"
#define WLAN0 "wlp58s0"


/* Appearance */
#define BEG "["
#define SEP "]["
#define END "]"
static const char* Months[12]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
static const char* WeekDays[7]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
static const char* ampmstyle[2]={"am","pm"};
static const char* net_downup="↓↑";
static const char* Xkb_group_indicator[2]={"us","ru"};


/* Structures */

typedef struct 
{
	double percent;
	int hours, mins;
	int is_chr;
} Battery;

typedef struct 
{
	int mon;
	int mday; 
	int wday; 
	int hour; 
	int min;
	int sec;
	int is_pm;
} Clock;

typedef struct 
{
	double in, out;
	char essid[IW_ESSID_MAX_SIZE+1];
} Network;

typedef struct 
{
	long int percent;
	int is_on;
} Volume;

/* Functions */

static Battery calc_battery();
static Clock calc_time();
static Network calc_net();
static Volume calc_volume_pulse();
static void* capture_layout(void*);
static void* precise_time(void*);
static void sig_usr1(int);
static void sig_usr2(int);


/* Global variables */

static pthread_t status_thread, kb_thread, min_thread;
static Volume vol; /* global because of signal capturing */

/* Pulseaudio calculated volume -- much better */
#include "pulse_vol.h"

/* Functions' bodies */

Battery
calc_battery() 
{
	FILE *file;
	int now, full, current;
	Battery ret;
	
	file=fopen("/sys/class/power_supply/"BAT"/charge_now","r");
	fscanf(file,"%d",&now);
	fclose(file);
	
	file=fopen("/sys/class/power_supply/"BAT"/charge_full","r");
	fscanf(file,"%d",&full);
	fclose(file);

	file=fopen("/sys/class/power_supply/"BAT"/current_now","r");
	fscanf(file,"%d",&current);
	fclose(file);

	file=fopen("/sys/class/power_supply/"AC"/online","r");
	fscanf(file,"%d",&ret.is_chr);
	fclose(file);

	if (ret.is_chr==1) {
		ret.hours=(full-now)/current;
		ret.mins=(full-now)%current*60/current;
	}	else {
		ret.hours=now/current;
		ret.mins=now%current*60/current;
	}
	ret.percent=now*100.0/full;

	return ret;
}

Clock
calc_time() 
{
	Clock ret;
	struct timeval t;
	gettimeofday(&t, NULL);
	struct tm local;
	local = *localtime(&t.tv_sec);
	ret.mon = local.tm_mon;
	ret.mday = local.tm_mday;
	ret.wday = local.tm_wday; 
	ret.hour = local.tm_hour;
	ret.min = local.tm_min;
	ret.sec = local.tm_sec;
	ret.is_pm = ret.hour>=12 ? 1:0;
	ret.hour -= 12*ret.is_pm;
	if (ret.hour==0) ret.hour=12;
	return ret;
}

Network
calc_net() 
{
	Network ret;
	static long int in1, in2, out1, out2;
	static struct timeval now, old;
	FILE *file;

	old=now;
	gettimeofday(&now,NULL);
	in1=in2; out1=out2;
	
	file=fopen("/sys/class/net/"WLAN0"/statistics/rx_bytes","r");
	fscanf(file,"%ld",&in2);
	fclose(file);

	file=fopen("/sys/class/net/"WLAN0"/statistics/tx_bytes","r");
	fscanf(file,"%ld",&out2);
	fclose(file);

	ret.in=(in2-in1)>>10; 
	ret.in/=1024*(now.tv_sec+now.tv_usec*1e-6-old.tv_sec-old.tv_usec*1e-6);
	ret.out=(out2-out1)>>10; 
	ret.out/=1024*(now.tv_sec+now.tv_usec*1e-6-old.tv_sec-old.tv_usec*1e-6);

	int skfd;
	struct iwreq wrq;
	memset(&wrq, 0, sizeof(struct iwreq));
  wrq.u.essid.length = IW_ESSID_MAX_SIZE + 1;
	snprintf(wrq.ifr_name, sizeof(wrq.ifr_name), "%s", WLAN0);
	skfd = socket(AF_INET, SOCK_DGRAM, 0);
  memset(ret.essid, 0, sizeof(ret.essid));
	wrq.u.essid.pointer = ret.essid;
	ioctl(skfd,SIOCGIWESSID, &wrq);
	close(skfd);

	return ret;
}

Volume
calc_volume_pulse()
{
	Volume ret;
	get_pulse_volume(&ret.percent, &ret.is_on);
	/* functions in pulse_vol.h send SIGUSR1 to status_thread */
	return ret;
}

void*
capture_layout(void* voidlayout)
{
	Display* d;
	XEvent e;
	XkbEvent ke;
	int event_type;
	int *layout = voidlayout;

	d = XOpenDisplay(NULL);
	XkbQueryExtension(d, 0, &event_type, 0, 0, 0);
  XkbSelectEvents(d, XkbUseCoreKbd, XkbStateNotifyMask, XkbStateNotifyMask);
	XSync(d,False);

	while (1)
	{
		XNextEvent(d,&e);
		if (e.type == event_type) {
			ke = (XkbEvent) e;
			if (ke.any.xkb_type == XkbStateNotify)
				if (ke.state.group != *layout) {
						*layout = ke.state.group;
						pthread_kill(status_thread, SIGUSR2);
					}
		}
	}
}

void*
precise_time(void* voidnow)
{
	int interval = 60;
	struct timeval current; 
	struct timespec wait; 
	Clock* now = voidnow;
	while (1)
	{
		gettimeofday(&current, NULL);
		wait.tv_sec = interval - 1 - (current.tv_sec % interval);
		wait.tv_nsec = (10e5 - current.tv_usec) * 1000;
		nanosleep(&wait, NULL);
		*now = calc_time();
		pthread_kill(status_thread, SIGUSR2);
	}
}

void
sig_usr1(int signal)
{
	vol = calc_volume_pulse();
}

void
sig_usr2(int signal)
{ } /* Does nothing, but still interrupts sleep() */


/* Main */

int 
main() 
{
	const int net_len=IW_ESSID_MAX_SIZE+20, vol_len=10, kb_len=4, bat_len=20, time_len=20;
	const int status_len=net_len+vol_len+kb_len+bat_len+time_len+20;
	char net_str[net_len], vol_str[vol_len], kb_str[kb_len], bat_str[bat_len], time_str[time_len];
	char status_str[status_len];

	Battery bat;
	Clock now;
	int kb_layout;
	Network net;

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
	pthread_create(&min_thread, NULL, precise_time, &now);
	
	bat = calc_battery();
	now = calc_time();
	net = calc_net();
	vol = calc_volume_pulse();
	
	/* Infinite loop begins */
	while (1) 
	{
		if (count%5==0) bat = calc_battery();
		//if (count%5==0) now = calc_time();
		net = calc_net();
		count++;

		snprintf(net_str, net_len, "%s:%.2lf%s%.2lf", net.essid, \
				(net.in>0)? net.in:0, net_downup, (net.out>0)?net.out:0);
		
		snprintf(vol_str, vol_len, "%s:%s%ld%s", vol.is_on?"V":"M", \
				vol.percent<10?" ":"", vol.percent, vol.percent<100?"%":"");
		
		snprintf(kb_str, kb_len, "%s", Xkb_group_indicator[kb_layout]);
		
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

		//printf("%s\n", status_str);
		//fflush(stdout);
		sleep(1);
	}
	/* Infinite loop ends */
	
	pthread_join(kb_thread, NULL);

	return 0;
}

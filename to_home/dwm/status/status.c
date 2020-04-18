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
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <iwlib.h>
#include <pthread.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <alsa/asoundlib.h>

#define BAT_PATH "/sys/class/power_supply/BAT0/"
#define AC_PATH "/sys/class/power_supply/AC/"
#define WIFI_STAT_PATH "/sys/class/net/wlp58s0/statistics/"
#define BRIGHTNESS_PATH "/sys/class/backlight/intel_backlight/"


/* Appearance */
#define BEG "["
#define SEP "]["
#define END "]"
static const char *Months[12]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
static const char *WeekDays[7]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
static const char *ampmstyle[2]={"am","pm"};
static const char *net_downup="↓↑";
static const char *Xkb_group_indicator[2]={"🇺🇸","🇷🇺"};
static const char *Xkb_group_indicator_text[2]={"us","ru"};
static const char *brightness_max_indicator[2]={"🔅","🔆"};
static const char *volume_icons[4]={"🔇","🔈","🔉","🔊"};

static const char *scale_blocks[31]=
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

static const char *scale_dots[6]=
	{"•••••","•••• ·","••• · ·","•• · · ·","• · · · ·"," · · · · ·"};


/* Structures */

typedef struct 
{
	double percent;
	int hours, mins;
	int is_chr;
} Battery;

typedef struct 
{
	struct tm now;
	int is_pm;
} Timenow;

typedef struct 
{
	double in, out;
	char essid[IW_ESSID_MAX_SIZE+1];
} Network;

typedef struct 
{
	double percent;
	int is_max;
} Brightness;

typedef struct 
{
	long int percent;
	int is_on;
	int scale;
	int icon;
} Volume;

/* Functions */

static void color(const char[7]);
static void background(const char[7]);
static Battery calc_battery();
static Timenow calc_time();
static Network calc_netspeed();
//static int calc_layout(Display *);
static Brightness calc_brightness();
//static Volume calc_volume();
static Volume calc_volume_pulse();
static void* capture_layout(void*);
static void sig_usr1(int);
static void sig_usr2(int);


/* Global variables */

//static pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
static pthread_t status_thread, kb_thread;
static Volume vol; /* global because of signal capturing */
static int kb_layout;

/* Pulseaudio calculated volume -- much better */
#include "pulse_vol.h"

/* Functions' bodies */

void 
color(const char thecolor[7]) 
{
	printf(" foreground=\"#%s\"", thecolor);
	return ;
}

void 
background(const char thecolor[7]) 
{
	printf(" background=\"#%s\"", thecolor);
	return ;
} 

Battery
calc_battery() 
{
	FILE *file;
	int now, full, current;
	Battery tmp;
	
	file=fopen(BAT_PATH"charge_now","r");
	fscanf(file,"%d",&now);
	fclose(file);
	
	file=fopen(BAT_PATH"charge_full","r");
	fscanf(file,"%d",&full);
	fclose(file);

	file=fopen(BAT_PATH"current_now","r");
	fscanf(file,"%d",&current);
	fclose(file);

	file=fopen(AC_PATH"online","r");
	fscanf(file,"%d",&tmp.is_chr);
	fclose(file);

	if (tmp.is_chr==1) {
		tmp.hours=(full-now)/current;
		tmp.mins=(full-now)%current*60/current;
	}
	else {
		tmp.hours=now/current;
		tmp.mins=now%current*60/current;
	}
	tmp.percent=now*100.0/full;

	return tmp;
}

Timenow
calc_time() 
{
	Timenow tmp;
	time_t t;
	time(&t);
	tmp.now = *localtime(&t);
	tmp.is_pm = tmp.now.tm_hour>=12 ? 1:0;
	tmp.now.tm_hour -= 12*tmp.is_pm;
	if (tmp.now.tm_hour==0) tmp.now.tm_hour=12;
	return tmp;
}

Network
calc_netspeed() 
{
	Network tmp;
	static long int in1, in2, out1, out2;
	static struct timeval now, old;
	FILE *file;

	old=now;
	gettimeofday(&now,NULL);
	in1=in2; out1=out2;
	
	file=fopen(WIFI_STAT_PATH"rx_bytes","r");
	fscanf(file,"%ld",&in2);
	fclose(file);

	file=fopen(WIFI_STAT_PATH"tx_bytes","r");
	fscanf(file,"%ld",&out2);
	fclose(file);

	tmp.in=(in2-in1)>>10; 
	tmp.in/=1024*(now.tv_sec+now.tv_usec*1e-6-old.tv_sec-old.tv_usec*1e-6);
	tmp.out=(out2-out1)>>10; 
	tmp.out/=1024*(now.tv_sec+now.tv_usec*1e-6-old.tv_sec-old.tv_usec*1e-6);

	int skfd;
	skfd = iw_sockets_open();
	struct iwreq wrq;

  memset(tmp.essid, 0, sizeof(tmp.essid));
  wrq.u.essid.pointer = (caddr_t) tmp.essid;
  wrq.u.essid.length = IW_ESSID_MAX_SIZE + 1;
  wrq.u.essid.flags = 0;
  iw_get_ext(skfd, "wlp58s0", SIOCGIWESSID, &wrq);
	iw_sockets_close(skfd);

	return tmp;
}

/*int
calc_layout(Display *display) 
{
	XkbStateRec kb_state;
	XkbGetState(display, XkbUseCoreKbd, &kb_state);
	return kb_state.group;
}*/

Brightness
calc_brightness() 
{
	Brightness tmp;
	FILE* file;
	int max, actual;

	file=fopen(BRIGHTNESS_PATH"max_brightness","r");
	fscanf(file,"%d",&max);
	fclose(file);
	
	file=fopen(BRIGHTNESS_PATH"actual_brightness","r");
	fscanf(file,"%d",&actual);
	fclose(file);

	tmp.percent=100.0*actual/max;
	tmp.is_max = tmp.percent>99 ? 1:0;

	return tmp;
}

/*Volume 
calc_volume() 
{
	Volume tmp;
	
	snd_mixer_t* handle;
	snd_mixer_elem_t* elem;
	snd_mixer_selem_id_t* sid;
	long min, max;

	snd_mixer_selem_id_alloca(&sid);
	snd_mixer_selem_id_set_index(sid, 0);
	snd_mixer_selem_id_set_name(sid, "Master");

	snd_mixer_open(&handle, 0);
	snd_mixer_attach(handle, "default");
	snd_mixer_selem_register(handle, NULL, NULL);
	snd_mixer_load(handle);
	elem = snd_mixer_find_selem(handle, sid);
	snd_mixer_selem_get_playback_volume_range (elem, &min, &max);
	
	snd_mixer_selem_get_playback_volume(elem, 0, &tmp.percent);
	snd_mixer_selem_get_playback_switch(elem, 0, &tmp.is_on);
	
	tmp.percent = 100*(tmp.percent-min)/(max-min);

	snd_mixer_close(handle);
	//snd_mixer_selem_id_free(sid); // this should not be commented, but doesn't work uncommented 

	if (tmp.percent>95) tmp.scale=0;
	else if (tmp.percent>75) tmp.scale=1;
	else if (tmp.percent>50) tmp.scale=2;
	else if (tmp.percent>25) tmp.scale=3;
	else if (tmp.percent>0)  tmp.scale=4;
	else tmp.scale=5;

	if (!tmp.is_on) tmp.icon=0;
	else if (tmp.percent<30) tmp.icon=1;
	else if (tmp.percent<90) tmp.icon=2;
	else tmp.icon=3;

	return tmp;
}*/

Volume
calc_volume_pulse()
{
	Volume tmp;

	get_pulse_volume(&tmp.percent, &tmp.is_on);
	/* functions in pulse_vol.h send SIGUSR1 to status_thread */

	if (tmp.percent>95) tmp.scale=0;
	else if (tmp.percent>75) tmp.scale=1;
	else if (tmp.percent>50) tmp.scale=2;
	else if (tmp.percent>25) tmp.scale=3;
	else if (tmp.percent>0)  tmp.scale=4;
	else tmp.scale=5;

	if (!tmp.is_on) tmp.icon=0;
	else if (tmp.percent<30) tmp.icon=1;
	else if (tmp.percent<90) tmp.icon=2;
	else tmp.icon=3;

	return tmp;
}

void*
capture_layout(void* unused)
{
	Display* d;
	XEvent e;
	XkbEvent ke;
	int event_type;
	//int kb_layout_old=0;

	d = XOpenDisplay(NULL);
	XkbQueryExtension(d, 0, &event_type, 0, 0, 0);
  XkbSelectEvents(d, XkbUseCoreKbd, XkbStateNotifyMask, XkbStateNotifyMask);
	XSync(d,False);

	while (1)
	{
		//pthread_mutex_lock(&mutex1);
		XNextEvent(d,&e);
		//kb_layout_old = kb_layout;
		if (e.type == event_type) {
			ke = (XkbEvent) e;
			if (ke.any.xkb_type == XkbStateNotify)
				if (ke.state.group != kb_layout) {
						//printf("TEST"); fflush(stdout);
						pthread_kill(status_thread, SIGUSR2);
						kb_layout = ke.state.group;
					}
		}
		//pthread_mutex_unlock(&mutex1);
	}

}

void
sig_usr1(int signal)
{
	//vol = calc_volume();
	vol = calc_volume_pulse();
}

void
sig_usr2(int signal)
{ } /* Does nothing, but still interrupts sleep() */


/* Main */

int 
main() 
{
	const int net_len=IW_ESSID_MAX_SIZE+20, brt_len=10, vol_len=10, kb_len=4, bat_len=20, time_len=20;
	const int status_len=net_len+brt_len+vol_len+kb_len+bat_len+time_len+20;
	char net_str[net_len], brt_str[brt_len], vol_str[vol_len], kb_str[kb_len], bat_str[bat_len], time_str[time_len];
	char status_str[status_len];

	Battery bat;
	Timenow now;
	Network net;
	Brightness bright;

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

	pthread_create(&kb_thread, NULL, capture_layout, NULL);
	
	bat = calc_battery();
	now = calc_time();
	net = calc_netspeed();
	//kb_layout = calc_layout(display);
	bright = calc_brightness();
	vol = calc_volume_pulse();
	
	/* Infinite loop begins */
	while (1) 
	{
		if (count%5==0) bat = calc_battery();
		if (count%5==0) now = calc_time();
		net = calc_netspeed();
		//kb_layout = calc_layout(display);
		bright = calc_brightness();
		//vol = calc_volume();
		count++;

		//printf(">%s: In %.3lf MB/s Out %.3lf MB/s"END, essid, (in>0)? in:0, (out>0)?out:0);
		snprintf(net_str, net_len, "%s:%.2lf%s%.2lf", net.essid, (net.in>0)? net.in:0, net_downup, (net.out>0)?net.out:0);
		
		//printf(">%d%s"END, bright.percent, brightness_max_indicator[bright.is_max]);
		snprintf(brt_str, brt_len, "%s", bright.is_max? "Bright"SEP:"");
		
		snprintf(vol_str, vol_len, "%s:%s%ld%s", vol.is_on?"V":"M", vol.percent<10?" ":"", vol.percent, vol.percent<100?"%":"");
		//printf("%s", scale_dots[vol.scale]);
		
		snprintf(kb_str, kb_len, "%s", Xkb_group_indicator_text[kb_layout]);
		
		snprintf(bat_str, bat_len, "%.2lf%%%s %02i:%02i%s", bat.percent, (bat.is_chr? "+" : ""), bat.hours, bat.mins, (bat.percent>10 || bat.is_chr)?"":(count%2==0?"":" LOW!") );
		
		snprintf(time_str, time_len, "%s %s %d %d:%02d %s", WeekDays[now.now.tm_wday], Months[now.now.tm_mon], now.now.tm_mday, now.now.tm_hour, now.now.tm_min, ampmstyle[now.is_pm]);

		snprintf(status_str, status_len, BEG"%s"SEP"%s%s"SEP"%s"SEP"%s"SEP"%s"END, net_str, brt_str, vol_str, kb_str, bat_str, time_str);
		
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

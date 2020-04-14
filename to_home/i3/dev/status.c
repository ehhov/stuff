/*
--------------------- NOTES ---------------------
json file format in i3bar 
  https://i3wm.org/docs/i3bar-protocol.html

the full information about power devices is located in uevent file
the paths to batteries can be found with 
  find / -type d -name power_supply
the paths are: 
  /sys/devices/LNXSYSTM:00/LNXSYBUS:00/ACPI0003:00/power_supply/AC
  /sys/devices/LNXSYSTM:00/LNXSYBUS:00/PNP0C0A:00/power_supply/BAT0

for many symbols see 
  https://jrgraphix.net/r/Unicode/
	https://emojipedia.org/
useful symbols: 
  🔇🔈🔉🔊 -- emoji style speaker
       -- text style speaker
  ░▒▓█
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
#include <iwlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <alsa/asoundlib.h>

#define FULL_TEXT "{\"full_text\": \""
#define SHORT_TEXT "\"short_text\": \""
#define TEXT_END "\","
#define BAT_PATH "/sys/class/power_supply/BAT0/"
#define AC_PATH "/sys/class/power_supply/AC/"
#define WIFI_STAT_PATH "/sys/class/net/wlp58s0/statistics/"
#define BRIGHTNESS_PATH "/sys/class/backlight/intel_backlight/"
#define XKB_GROUP_0 "🇺🇸"
#define XKB_GROUP_1 "🇷🇺"

int count=0;

struct the_battery {
	double percent;
	int hours, mins;
	int chr;
};

struct the_battery battery;

const char Months[12][4]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
const char WeekDay[7][4]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
time_t t;
struct tm times;
char ampm[3];
int hour;

double in, out;
long int in1, in2, out1, out2;
struct timeval net_now, net_old;
char essid[IW_ESSID_MAX_SIZE + 1];

static Display *display;
XkbStateRec kb_state;

int brightness_percent;

long int volume_percent;
int volume_ison;
snd_mixer_t* handle;
snd_mixer_elem_t* elem;
snd_mixer_selem_id_t* sid;
long volume_min, volume_max;
char volume_scale[41];

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


/*XEvent e;
Display *d;
int xkbEventType, layout;
XkbEvent* xkbEvent;*/

void color (const char[7], int );
void background(const char[7]);
void calc_battery ();
void calc_time();
void calc_netspeed();
void calc_layout();
void calc_brightness();
void calc_volume();


int main (void) {
	//char the_time[20], the_battery[14];
	ampm[1]='m';	
	display = XOpenDisplay(NULL);

	snd_mixer_selem_id_alloca(&sid);
	
	snd_mixer_selem_id_set_index(sid, 0);
	snd_mixer_selem_id_set_name(sid, "Master");
	
	printf("{\"version\":1}\n[\n");
	
	while (1) {
		
		//if (count%50==0) calc_battery(); // for usleep(100000)
		//if (count%150==0) calc_time();
		//if (count%5==0) calc_netspeed();
		if (count%5==0) calc_battery(/*the_battery*/); // for sleep(1)
		if (count%15==0) calc_time(/*the_time*/);
		calc_netspeed();
		calc_layout();
		calc_brightness();
		calc_volume();

		{
			printf("[\n");

			printf(FULL_TEXT"%s: In %.3lf MB/s Out %.3lf MB/s"TEXT_END, essid, (in>0)? in:0, (out>0)?out:0);
			//printf(FULL_TEXT"%s: %.2lf↓↑%.2lf"TEXT_END, essid, in, out);
			color("666666",1);

			//printf(FULL_TEXT"%d%s"TEXT_END, brightness_percent, brightness_percent>80? "🔆":"🔅");
			printf(FULL_TEXT"%s"TEXT_END,brightness_percent>90? "🔆":"🔅");
			color("ff8f00",1);

			printf(FULL_TEXT);
			if (!volume_ison) printf("🔇");
			else {
				if (volume_percent>30) printf("🔊");
				else if (volume_percent>0) printf("🔉");
				else printf("🔈");
			}
			//printf("%ld%%", volume_percent);
			//printf(" %s", volume_scale);
			printf(TEXT_END);
			color("55ccff",1);
			//color("ffffff",1);


			printf(FULL_TEXT"%s"TEXT_END, kb_state.group==0? XKB_GROUP_0:XKB_GROUP_1 );
			color("ffffff",1);			

			printf(FULL_TEXT"%.2lf%%%s %02i:%02i"TEXT_END, battery.percent, (battery.chr? "+" : ""), battery.hours, battery.mins);
			//printf(FULL_TEXT "%s",the_battery);
			if (battery.percent>75)
				color("00ff00",1);
			else if (battery.percent>40)
				color("aadd00", 1);
			else if (battery.percent>15)
				color("ffcc00",1);
			else
				color("ff5500",1);

			printf(FULL_TEXT"%s %s %d %d:%02d %s "TEXT_END, WeekDay[times.tm_wday], Months[times.tm_mon], times.tm_mday, hour, times.tm_min, ampm);
			color("ffffff",0);
		
			printf("],\n");
			fflush(stdout);
			//usleep(100000); // 0.1 s
			sleep(1);
			count++;
		}
	}

	
	return 0;
}

void color (const char colour[7], int end) {
	printf("\"color\": \"#%s\"}",colour);
	if (end!=0)	printf(",\n");
	else printf("\n");
	return ;
}

void background (const char colour[7]) {
	printf("\"background\": \"#%s\", ", colour);
	return ;
} 

void calc_battery (/*char a[14]*/) {
	FILE *file;
	int now, full, current;
	
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
	fscanf(file,"%d",&battery.chr);
	fclose(file);

	if (battery.chr==1) {
		battery.hours=(full-now)/current;
		battery.mins=(full-now)%current*60/current;
	}
	else {
		battery.hours=now/current;
		battery.mins=now%current*60/current;
	}
	battery.percent=now*100.0/full;

	return ;
}

void calc_time (/*char a[20]*/) {
	time(&t);
	times=*localtime(&t);
	if(times.tm_hour>=12) {ampm[0]='p'; hour=times.tm_hour-12;}
	else {ampm[0]='a';hour=times.tm_hour;}
	if (hour==0) hour+=12;
	//sprintf(a,"%s %s %d %d:%02d %s", WeekDay[(int)times.tm_wday], Months[(int)times.tm_mon], times.tm_mday, hour, times.tm_min, ampm);
	return;
}

void calc_netspeed() {
	FILE *file;

	net_old=net_now;
	gettimeofday(&net_now,NULL);
	in1=in2; out1=out2;
	
	file=fopen(WIFI_STAT_PATH"rx_bytes","r");
	fscanf(file,"%ld",&in2);
	fclose(file);

	file=fopen(WIFI_STAT_PATH"tx_bytes","r");
	fscanf(file,"%ld",&out2);
	fclose(file);

	in=(in2-in1)>>10; in/=1024*(net_now.tv_sec+net_now.tv_usec*1e-6-net_old.tv_sec-net_old.tv_usec*1e-6);
	out=(out2-out1)>>10; out/=1024*(net_now.tv_sec+net_now.tv_usec*1e-6-net_old.tv_sec-net_old.tv_usec*1e-6);
	
	int skfd;

	skfd = iw_sockets_open();

	struct iwreq wrq;
  
  // Make sure ESSID is always NULL terminated 
  memset(essid, 0, sizeof(essid));

  // Get ESSID 
  wrq.u.essid.pointer = (caddr_t) essid;
  wrq.u.essid.length = IW_ESSID_MAX_SIZE + 1;
  wrq.u.essid.flags = 0;
  iw_get_ext(skfd, "wlp58s0", SIOCGIWESSID, &wrq);

	iw_sockets_close(skfd);

	return ;
}

void calc_layout() {
	XkbGetState(display, XkbUseCoreKbd, &kb_state);
	return ;
}

void calc_brightness() {
	FILE* file;
	int max, actual;

	file=fopen(BRIGHTNESS_PATH"max_brightness","r");
	fscanf(file,"%d",&max);
	fclose(file);
	
	file=fopen(BRIGHTNESS_PATH"actual_brightness","r");
	fscanf(file,"%d",&actual);
	fclose(file);

	brightness_percent=100*actual/max;

	return ;
}

void calc_volume() {
	int vol;
	
	snd_mixer_open(&handle, 0);
	snd_mixer_attach(handle, "default");
	snd_mixer_selem_register(handle, NULL, NULL);
	snd_mixer_load(handle);
	elem = snd_mixer_find_selem(handle, sid);
	snd_mixer_selem_get_playback_volume_range (elem, &volume_min, &volume_max);
	
	snd_mixer_selem_get_playback_volume(elem, 0, &volume_percent);
	snd_mixer_selem_get_playback_switch(elem, 0, &volume_ison);
	
	volume_percent = 100*(volume_percent-volume_min)/(volume_max-volume_min);

	snd_mixer_close(handle);

	vol=volume_percent/3.3;
	/*for (int i=0; i<=vol/2; i+=1) {
		if (vol-2*i==0) volume_scale[i]=' ';
		else if (vol-2*i==1) volume_scale[i]='-';
		else volume_scale[i]='=';
	}
	for (int i=vol/2+1; i<10; i+=1)
		volume_scale[i]=' ';
	*/

	strcpy(volume_scale,scale_blocks[vol]);
	
	return ;
}

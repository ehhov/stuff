/* Appearance */
#define BEG "["
#define SEP "]["
#define END "]"
#define SP "<span "
#define PS "</span>"
static const char* Months[12]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
static const char* WeekDays[7]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
static const char* ampmstyle[2]={"am","pm"};
static const char* net_downup="↓↑";
static const char* Xkb_group_text[2]={"us","ru"};
static const int interval = 5;

char *Xkb_group_icon[2]={"🇺🇸","🇷🇺"};
char *volume_icon[4]={"🔇","🔈","🔉","🔊"};

/* Variables */
const char* battery = "BAT0";
const char* wlan = "wlp58s0";

/*
--------------------- NOTES ---------------------
battery instance can be found in 
	/sys/class/power_supply/
wlan interface can be found in 
	/sys/class/net/

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

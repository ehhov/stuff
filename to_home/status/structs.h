typedef struct Battery Battery;
struct Battery
{
	double percent;
	int hours, mins;
	int is_chr;
};

typedef struct Clock Clock;
struct Clock
{
	int mon;
	int mday; 
	int wday; 
	int hour; 
	int min;
	int sec;
	int is_pm;
};

#include <linux/wireless.h>
extern const int id_len;
typedef struct Network Network;
struct Network
{
	double in, out;
	char essid[IW_ESSID_MAX_SIZE+1];
};

typedef struct Volume Volume;
struct Volume
{
	long int percent;
	int is_on;
	int icon;
};

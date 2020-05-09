#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include <ifaddrs.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/wireless.h>

#include "structs.h"

static const int len = 50;
const int id_len = IW_ESSID_MAX_SIZE+1;

Network
netspeed(const char* wlan) 
{
	Network ret;
	static long int in1, in2, out1, out2;
	static struct timeval now, old;
	char path[len];
	FILE *file;

	old=now;
	gettimeofday(&now, NULL);
	in1=in2; out1=out2;
	
	snprintf(path, len, "/sys/class/net/%s/statistics/rx_bytes", wlan);
	file=fopen(path,"r");
	fscanf(file,"%ld",&in2);
	fclose(file);

	snprintf(path, len, "/sys/class/net/%s/statistics/tx_bytes", wlan);
	file=fopen(path,"r");
	fscanf(file,"%ld",&out2);
	fclose(file);

	ret.in=(in2-in1)>>10; 
	ret.in/=1024*(now.tv_sec+now.tv_usec*1e-6-old.tv_sec-old.tv_usec*1e-6);
	ret.out=(out2-out1)>>10; 
	ret.out/=1024*(now.tv_sec+now.tv_usec*1e-6-old.tv_sec-old.tv_usec*1e-6);

	return ret;
}

const char* 
essid(const char* wlan)
{
	static char name[IW_ESSID_MAX_SIZE+1];
	int skfd;
	struct iwreq wrq;
	memset(&wrq, 0, sizeof(struct iwreq));
  wrq.u.essid.length = IW_ESSID_MAX_SIZE + 1;
	snprintf(wrq.ifr_name, sizeof(wrq.ifr_name), "%s", wlan);
	skfd = socket(AF_INET, SOCK_DGRAM, 0);
  memset(name, 0, sizeof(name));
	wrq.u.essid.pointer = name;
	ioctl(skfd,SIOCGIWESSID, &wrq);
	close(skfd);
	return name;
}

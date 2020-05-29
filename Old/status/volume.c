#include <unistd.h>
#include <pthread.h>
#include <signal.h>
extern pthread_t status_thread;
#include "volume_pulse.h"

#include "structs.h"

Volume
volume_pulse()
{
	Volume ret;
	get_pulse_volume(&ret.percent, &ret.is_on);
	/* functions in volume_pulse.h send SIGUSR1 to status_thread */
	if (!ret.is_on) ret.icon=0;
	else if (ret.percent<30) ret.icon=1;
	else if (ret.percent<90) ret.icon=2;
	else ret.icon=3;
	return ret;
}

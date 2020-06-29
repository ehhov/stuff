#include <stdio.h>
#include <time.h>

int
main()
{
	const char *ampmstyle[2]={"am", "pm"};
	struct timespec precise;
	struct tm local;
	int pm;
	/* usual time_t and time() is 0.0...01 second slow */
	clock_gettime(CLOCK_REALTIME, &precise);
	localtime_r(&precise.tv_sec, &local);
	pm = local.tm_hour>=12 ? 1:0;
	local.tm_hour -= 12*pm;
	if (local.tm_hour == 0)
		local.tm_hour = 12;

	return printf("%d:%02d:%02d%s", local.tm_hour, local.tm_min, local.tm_sec, ampmstyle[pm]);
}

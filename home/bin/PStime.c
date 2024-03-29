#include <stdio.h>
#include <time.h>

int
main()
{
	const char *ampmstyle[] = {"am", "pm"};
	int ispm;
	struct timespec precise;
	struct tm local;
	/* usual time_t and time() are 0.0...01 second slow */
	clock_gettime(CLOCK_REALTIME, &precise);
	localtime_r(&precise.tv_sec, &local);

	ispm = local.tm_hour < 12 ? 0 : 1;
	local.tm_hour -= 12 * ispm;
	if (local.tm_hour == 0)
		local.tm_hour = 12;

	return printf("%d:%02d:%02d%s", local.tm_hour, local.tm_min,
	                                local.tm_sec, ampmstyle[ispm]) < 0;
}

#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

extern pthread_t status_thread;

void*
capture_layout(void* voidlayout)
{
	Display* d;
	XEvent e;
	XkbEvent ke;
	int event_type;
	int* layout = voidlayout;
	*layout = 0;

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

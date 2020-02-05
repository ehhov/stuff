//gcc flags:  -Wall -O2 -lX11 

#include <stdio.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>

int main(int argc, char **argv)
{
	XEvent e;
	Display *d;
	XkbStateRec xkb_state;
	int group;

	if (!(d = XOpenDisplay(NULL))) {
		fprintf(stderr, "cannot open display\n");
		return 1;
	}

	XkbGetState(d, XkbUseCoreKbd, &xkb_state);
	group = xkb_state.group;

	printf("LOL %d",group);
	
	/*XKeysymToKeycode(d, XK_F1);
	
	int xkbEventType;
	XkbQueryExtension(d, 0, &xkbEventType, 0, 0, 0);
	XkbSelectEvents(d, XkbUseCoreKbd, XkbAllEventsMask, XkbAllEventsMask);
	XSync(d, False);
	
	while (1) {
		XNextEvent(d, &e);
		printf("type %d\n",e.type);
		if (e.type == xkbEventType) {
			XkbEvent* xkbEvent = (XkbEvent*) &e;
			if (xkbEvent->any.xkb_type == XkbStateNotify) {
				int lang = xkbEvent->state.group;
				if (lang == 1) {
					fprintf(stdout, "1\n");
					fflush(stdout);
				} else {
					fprintf(stdout, "0\n");
					fflush(stdout);
				}
			}
		}
		fflush(stdout);
	}*/
	
	return(0);
}

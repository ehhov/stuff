/* 
	The content of this file came from i3status source code
	https://github.com/i3/i3status/
	It aims at instant status info update on any action 
	upon volume: mute toggle, volume change, headphones 
	plug in, etc.
	Unfortunately, I don't understand what is going on here,
	but it works in my status in the same way as in i3status.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <pulse/pulseaudio.h>


#define DECOMPOSE_VOLUME(cvol) ((cvol) & ~(1 << 30))
#define DECOMPOSE_MUTED(cvol) (((cvol) & (1 << 30)) != 0)
#define COMPOSE_VOLUME_MUTE(vol, mute) ((vol) | ((mute) ? (1 << 30) : 0))
#define MAX_SINK_DESCRIPTION_LEN 128
#define DEFAULT_SINK_INDEX UINT32_MAX
#define APP_NAME "m"
#define APP_ID "org.m"
#define I3STATUS_VERSION "1.0"

#define TAILQ_ENTRY(type)                                          \
	struct {                                                         \
		struct type *tqe_next;  /* next element */                     \
		struct type **tqe_prev; /* address of previous next element */ \
	}
#define TAILQ_HEAD(name, type)                                  \
	struct name {                                               \
		struct type *tqh_first; /* first element */             \
		struct type **tqh_last; /* addr of last next element */ \
	}

#define TAILQ_HEAD_INITIALIZER(head) \
    { NULL, &(head).tqh_first }
#define TAILQ_FIRST(head) ((head)->tqh_first)
#define TAILQ_END(head) NULL
#define TAILQ_NEXT(elm, field) ((elm)->field.tqe_next)
#define TAILQ_FOREACH(var, head, field) \
	for ((var) = TAILQ_FIRST(head);       \
			 (var) != TAILQ_END(head);        \
		 	 (var) = TAILQ_NEXT(var, field))
#define TAILQ_INSERT_HEAD(head, elm, field)                             \
	do {                                                                \
		if (((elm)->field.tqe_next = (head)->tqh_first) != NULL)        \
			(head)->tqh_first->field.tqe_prev = &(elm)->field.tqe_next; \
		else                                                            \
			(head)->tqh_last = &(elm)->field.tqe_next;                  \
		(head)->tqh_first = (elm);                                      \
		(elm)->field.tqe_prev = &(head)->tqh_first;                     \
	} while (0)



/* Structures and other */
typedef struct index_info_s {
    char *name;
    uint32_t idx;
    int volume;
    char description[MAX_SINK_DESCRIPTION_LEN];
    TAILQ_ENTRY(index_info_s)
    entries;
} index_info_t;


/* Functions */
static int pulse_initialize(void);
static int description_pulseaudio(uint32_t, const char *, char buffer[MAX_SINK_DESCRIPTION_LEN]);
static int volume_pulseaudio(uint32_t, const char *);
static void pulseaudio_error_log(pa_context *);
static int pulseaudio_free_operation(pa_context *, pa_operation *);
static void subscribe_cb(pa_context *, pa_subscription_event_type_t, uint32_t, void *);
static void update_default_sink(pa_context *);
static void store_default_sink_cb(pa_context *, const pa_sink_info *, int, void *);
static void get_sink_info(pa_context *, uint32_t, const char *);
static void store_info_from_sink_cb(pa_context *, const pa_sink_info *, int, void *);
static int save_info(uint32_t, int, const char *, const char *);
void get_pulse_volume();


/* Global variables */
static pa_threaded_mainloop *main_loop = NULL;
static pa_mainloop_api *api = NULL;
static pa_context *context = NULL;
static int mainloop_thread_running = 0;
static int context_ready = 0;
static uint32_t default_sink_idx = DEFAULT_SINK_INDEX;
static pthread_mutex_t pulse_mutex = PTHREAD_MUTEX_INITIALIZER;
TAILQ_HEAD(tailhead, index_info_s) cached_info = TAILQ_HEAD_INITIALIZER(cached_info);


/* Functions' bodies */
int 
volume_pulseaudio(uint32_t sink_idx, const char *sink_name) 
{
	if (!context_ready || default_sink_idx == DEFAULT_SINK_INDEX)
		return -1;

	pthread_mutex_lock(&pulse_mutex);
	const index_info_t *entry;
	TAILQ_FOREACH(entry, &cached_info, entries) {
		if (sink_name) {
			if (!entry->name || strcmp(entry->name, sink_name)) {
				continue;
			}
		} else {
			if (entry->idx != sink_idx) {
				continue;
			}
		}
		int vol = entry->volume;
		pthread_mutex_unlock(&pulse_mutex);
		return vol;
	}
	pthread_mutex_unlock(&pulse_mutex);
	/* first time requires a prime callback call because we only get updates
	 * when the description or volume actually changes, but we need it to be
	 * correct even if it never changes */
	pa_threaded_mainloop_lock(main_loop);
	get_sink_info(context, sink_idx, sink_name);
	pa_threaded_mainloop_unlock(main_loop);
	/* show 0 while we don't have this information */
	return 0;
}

int 
description_pulseaudio(uint32_t sink_idx, const char *sink_name, char buffer[MAX_SINK_DESCRIPTION_LEN]) 
{
	if (!context_ready || default_sink_idx == DEFAULT_SINK_INDEX) {
		return 0;
	}

	pthread_mutex_lock(&pulse_mutex);
	const index_info_t *entry;
	TAILQ_FOREACH(entry, &cached_info, entries) {
		if (sink_name) {
			if (!entry->name || strcmp(entry->name, sink_name)) {
				continue;
			}
		} else {
			if (entry->idx != sink_idx) {
				continue;
			}
		}
		strncpy(buffer, entry->description, sizeof(entry->description) - 1);
		pthread_mutex_unlock(&pulse_mutex);
		buffer[sizeof(entry->description) - 1] = '\0';
		return 1;
	}
	pthread_mutex_unlock(&pulse_mutex);
	/* first time requires a prime callback call because we only get updates
	 * when the description or volume actually changes, but we need it to be
	 * correct even if it never changes */
	pa_threaded_mainloop_lock(main_loop);
	get_sink_info(context, sink_idx, sink_name);
	pa_threaded_mainloop_unlock(main_loop);
	/* show empty string while we don't have this information */
	buffer[0] = '\0';
	return 1;
}

static int 
save_info(uint32_t sink_idx, int new_volume, const char *new_description, const char *name) 
{
	pthread_mutex_lock(&pulse_mutex);
	index_info_t *entry;

	/* if this is NULL, gracefully handle and replace with empty-string */
	if (!new_description) {
		new_description = "";
		fprintf(stderr, "i3status: PulseAudio: NULL new_description provided\n");
	}

	TAILQ_FOREACH(entry, &cached_info, entries) {
		if (name) {
			if (!entry->name || strcmp(entry->name, name)) {
				continue;
			}
		} else {
			if (entry->idx != sink_idx) {
				continue;
			}
		}

		int changed = 0;

		if (new_volume != entry->volume) {
			entry->volume = new_volume;
			changed = 1;
		}

		if (strncmp(entry->description, new_description, sizeof(entry->description))) {
			strncpy(entry->description, new_description, sizeof(entry->description) - 1);
			entry->description[sizeof(entry->description) - 1] = '\0';
			changed = 1;
		}

		pthread_mutex_unlock(&pulse_mutex);
		return changed;
	}
	/* index not found, store it */
	entry = malloc(sizeof(*entry));
	TAILQ_INSERT_HEAD(&cached_info, entry, entries);
	entry->idx = sink_idx;
	entry->volume = new_volume;
	strncpy(entry->description, new_description, sizeof(entry->description) - 1);
	entry->description[sizeof(entry->description) - 1] = '\0';
	if (name) {
		entry->name = malloc(strlen(name) + 1);
		strcpy(entry->name, name);
	} else {
		entry->name = NULL;
	}
	pthread_mutex_unlock(&pulse_mutex);
	return 1;
}

static void 
store_info_from_sink_cb(pa_context *c, const pa_sink_info *info, int eol, void *userdata) 
{
	if (eol < 0) {
		if (pa_context_errno(c) == PA_ERR_NOENTITY)
			return;

		pulseaudio_error_log(c);
		return;
	}

	if (eol > 0)
		return;

	int avg_vol = pa_cvolume_avg(&info->volume);
	int vol_perc = avg_vol*100.0/PA_VOLUME_NORM+0.5;
	int composed_volume = COMPOSE_VOLUME_MUTE(vol_perc, info->mute);

	/* if this is the default sink we must try to save it twice: once with
	 * DEFAULT_SINK_INDEX as the index, and another with its proper value
	 * (using bitwise OR to avoid early-out logic) */
	if ((info->index == default_sink_idx && save_info(DEFAULT_SINK_INDEX, composed_volume, info->description, NULL)) | save_info(info->index, composed_volume, info->description, info->name)) {
		/* if the volume, mute flag or description changed, wake the main thread */
		pthread_kill(status_thread, SIGUSR1);
	}
}

static void 
get_sink_info(pa_context *c, uint32_t idx, const char *name) 
{
	pa_operation *o;

	if (name || idx == DEFAULT_SINK_INDEX) {
		o = pa_context_get_sink_info_by_name(c, name ? name : "@DEFAULT_SINK@", store_info_from_sink_cb, NULL);
	} else {
		o = pa_context_get_sink_info_by_index(c, idx, store_info_from_sink_cb, NULL);
	}
	if (o) {
		pulseaudio_free_operation(c, o);
	}
}

static void 
store_default_sink_cb(pa_context *c, const pa_sink_info *i, int eol, void *userdata) 
{
	if (i) {
		if (default_sink_idx != i->index) {
			/* default sink changed? */
			default_sink_idx = i->index;
			store_info_from_sink_cb(c, i, eol, userdata);//
		}
	}
}

static void 
update_default_sink(pa_context *c) 
{
	pa_operation *o = pa_context_get_sink_info_by_name(c,	"@DEFAULT_SINK@",	store_default_sink_cb, NULL);
	pulseaudio_free_operation(c, o);
}

static void 
subscribe_cb(pa_context *c, pa_subscription_event_type_t t, uint32_t idx, void *userdata) 
{
	if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) != PA_SUBSCRIPTION_EVENT_CHANGE)
		return;
	pa_subscription_event_type_t facility = t & PA_SUBSCRIPTION_EVENT_FACILITY_MASK;
	switch (facility) {
		case PA_SUBSCRIPTION_EVENT_SERVER:
			/* server change event, see if the default sink changed */
			update_default_sink(c);
			break;
		case PA_SUBSCRIPTION_EVENT_SINK:
			get_sink_info(c, idx, NULL);
			break;
		default:
			break;
	}
}

static void 
context_state_callback(pa_context *c, void *userdata) 
{
	switch (pa_context_get_state(c)) 
	{
		case PA_CONTEXT_UNCONNECTED:
		case PA_CONTEXT_CONNECTING:
		case PA_CONTEXT_AUTHORIZING:
		case PA_CONTEXT_SETTING_NAME:
		case PA_CONTEXT_TERMINATED:
		default:
			context_ready = 0;
			break;

		case PA_CONTEXT_READY: 
			{
				pa_context_set_subscribe_callback(c, subscribe_cb, NULL);
				update_default_sink(c); 

				pa_operation *o = pa_context_subscribe(c,	PA_SUBSCRIPTION_MASK_SINK | PA_SUBSCRIPTION_MASK_SERVER, NULL, NULL);
				if (!pulseaudio_free_operation(c, o)) 
					break;
				context_ready = 1;
			} break;

		case PA_CONTEXT_FAILED:
			{
				/* server disconnected us, attempt to reconnect */
				context_ready = 1;
				pa_context_unref(context);
				context = NULL;
			}	break;
	}
}

static int
pulseaudio_free_operation(pa_context *c, pa_operation *o) 
{
    if (o!=NULL)
        pa_operation_unref(o);
    else
        pulseaudio_error_log(c);
    /* return false if the operation failed */
    return o!=NULL;
}

static void 
pulseaudio_error_log(pa_context *c) 
{
  fprintf(stderr, "i3status: PulseAudio: %s\n", pa_strerror(pa_context_errno(c)));
}

int 
pulse_initialize(void) 
{
	if (!main_loop) 
	{
		main_loop = pa_threaded_mainloop_new();
		if (!main_loop)
			return 0;
	}
	if (!api) 
	{
		api = pa_threaded_mainloop_get_api(main_loop);
		if (!api)
			return 0;
	}
	if (!context) 
	{
		pa_proplist *proplist = pa_proplist_new();
		pa_proplist_sets(proplist, PA_PROP_APPLICATION_NAME, APP_NAME);
		pa_proplist_sets(proplist, PA_PROP_APPLICATION_ID, APP_ID);
		pa_proplist_sets(proplist, PA_PROP_APPLICATION_VERSION, I3STATUS_VERSION);
		context = pa_context_new_with_proplist(api, APP_NAME, proplist);
		pa_proplist_free(proplist);
		if (!context)
			return 0;
		pa_context_set_state_callback(context, context_state_callback, NULL);
		if (pa_context_connect(context, NULL, PA_CONTEXT_NOFAIL | PA_CONTEXT_NOAUTOSPAWN,	NULL) < 0) 
		{
			pulseaudio_error_log(context);
			return 0;
		}
		if (!mainloop_thread_running && pa_threaded_mainloop_start(main_loop) < 0) 
		{
			pulseaudio_error_log(context);
			pa_threaded_mainloop_free(main_loop);
			main_loop = NULL;
			return 0;
		}
		mainloop_thread_running = 1;
	}
	return 1;
}



/* The very bottom */
void 
get_pulse_volume(long int *ivolume, int *is_on)
{
	pulse_initialize();
	char description[MAX_SINK_DESCRIPTION_LEN];
	/*int success =*/ description_pulseaudio(DEFAULT_SINK_INDEX, NULL, description); 
	int cvolume = volume_pulseaudio(DEFAULT_SINK_INDEX, NULL); 
	*ivolume = DECOMPOSE_VOLUME(cvolume); 
	int muted = DECOMPOSE_MUTED(cvolume);
	*is_on = muted==0?1:0;

	/*if (ivolume >= 0 && success) {
		printf("%s:%ld\n",muted?"M":"V",*ivolume);
		fflush(stdout);
	} else {
		printf("Something went wrong.\n%s:%ld\n",muted?"M":"V",*ivolume);
		fflush(stdout);
	}*/
}

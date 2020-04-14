#include <stdio.h>
#include <alsa/asoundlib.h>

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


int main () {
	int lol;
	long int volume_percent;
	int volume_ison;
	snd_mixer_t* handle;
	snd_mixer_elem_t* elem;
	snd_mixer_selem_id_t* sid;
	long volume_min, volume_max;
	
	snd_mixer_selem_id_alloca(&sid);
	
	snd_mixer_selem_id_set_index(sid, 0);
	snd_mixer_selem_id_set_name(sid, "Master");

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

	lol=volume_percent/3.3;

	printf("Volume: %s (%s)", scale_blocks[lol], volume_ison? "on":"off");

	return 0;
}

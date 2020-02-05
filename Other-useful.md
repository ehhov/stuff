## Apps and packages
- CUPS, Chrome, Emacs, feh, Firefox, GCC, Git, Okular, Python, rxvt-unicode, Spectacle, Telegram Desktop, Thunderbird, VIM, VIM-X11, VLC, XTerm
- compton-tryone, i3, dmenu, Openbox, sddm, tint2
- alsa-lib,-devel, libX11,-devel, libXft,-devel, libXinerama,-devel, 
- dialog, libinput, iw, wpa\_supplicant, mesa, X11: xorg-server, -server-utils, -xinit, intel-driver, libinput-driver, xbacklight, alsa
- flac, libinput-gestures, xclip, simplescreenrecorder, maim
- DejaVu, Terminus, Iosevka (included in `fonts`)
 
- GNUplot, OpenSSH, ROOT

## Fix hissing sound in headphones
For alsamixer
``` bash
amixer -c PCH cset 'name=Headphone Mic Boost Volume' 1
```
For PulseAudio
``` bash
/usr/share/pulseaudio/alsa-mixer/paths/analog-input-headphone-mic.conf
[Element Headphone Mic Boost]
required-any = any
switch = select
# Replace "volume = merge" by:
volume = 1
override-map.1 = all
override-map.2 = all-left,all-right

/usr/share/pulseaudio/alsa-mixer/paths/analog-input-internal-mic.conf
[Element Headphone Mic Boost]
switch = off
# Replace "volume = off" by:
volume = 1
```

## CUPS configuration
1. Create group lpadmin.
1. Edit SystemGroup line in /etc/cups/cups-files.conf – add lpadmin. 
1. Add user to group admin. 
1. Type localhost:631/ in browser. 
1. Add printer smb://guest@192.168.1.1/Xerox3250. (the format is thing://guest@IP/name)
1. Use driver ph3250.ppd. Set yes in Share checkbox. 


## Cool dwm patches
Better patches are marked with +, not working with -, very complicated with ?

+-`centeredwindowname`, -`combo`, `fakefullscreen`, `flextile`, +`focusonclick`, `focusurgent`, +?`keymodes`, `noborder`, +`pango`, +`pertag`, ++?`systray`, `tilegap`, `uselessgap`


## Links
- [XBM online editor](https://xbm.jazzychad.net/),
- [Image color picker](https://html-color-codes.info/colors-from-image/),
- [Openbox-patched with rounded corners](https://github.com/dylanaraps/openbox-patched),
- [Color configuration for i3 online](https://thomashunter.name/i3-configurator/),
- Themes for Openbox: [one](https://github.com/fikriomar16/OBTheme-Collections), [two](https://github.com/addy-dclxvi/openbox-theme-collections)
- [A better Compton from tryone144](https://github.com/tryone144/compton)

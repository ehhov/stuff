# Apps and packages
- Necessary: CUPS, GCC, Git, Python, VIM, VIM-X11, XTerm, xbacklight, ALSA, PulseAudio

- Common choices: Firefox, Chrome, feh, Emacs, rxvt-unicode, VLC

- My choices: Okular, qpdfview-qt5, Thunar, maim, xclip, Thunderbird, Telegram Desktop, simplescreenrecorder, klavaro, libinput-gestures

- UI relating: sddm, dmenu, dwm (from this repository), Openbox, tint2, compton-tryone, i3

- Necessary for DWM and other 'developer' work: alsa-lib,-devel, libX11,-devel, libXft,-devel, libXinerama,-devel

- For minimal distros: dialog, libinput, iw, wpa\_supplicant, mesa, X11: xorg-server, -server-utils, -xinit, intel-driver, libinput-driver

- Fonts: DejaVu, Terminus, Iosevka, Arimo (some are included in `thicc/fonts`)
 
- Relating to Study: GNUplot, OpenSSH, ROOT, Ostap

# Various instructions 
## useradd, usermod, groupadd
`useradd -m name` -m created the /home/name directory

`groupadd name` 

`gpasswd -a user group` to add and `-d user` to remove. 


## Making xdg-mime work properly 
Edit `/usr/bin/xdg-mime` by replacing all 

``` mv $file.new $file ```

with

``` cat $file.new > $file ```

But this doesn't help all that much.

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
1. Create a group named lpadmin.
1. Edit SystemGroup line in /etc/cups/cups-files.conf – add lpadmin. 
1. Add the user to group lpadmin. 
1. Type localhost:631/ in a browser. 
1. Add printer smb://guest@192.168.1.1/Xerox3250. (the format is thing://guest@IP/name)
1. Use driver ph3250.ppd. Set yes in Share checkbox. 


## Cool dwm patches
Better patches are marked with +, not working with -, very complicated with ?

+-`centeredwindowname`, -`combo`, `fakefullscreen`, `flextile`, +`focusonclick`, `focusurgent`, +?`keymodes`, `noborder`, +`pango`, +`pertag`, ++?`systray`, `tilegap`, `uselessgap`


## Links
- [XBM online editor](https://xbm.jazzychad.net/),
- [Image color picker](https://html-color-codes.info/colors-from-image/),
- [A better image color picker](https://image-color.com/),
- [Openbox-patched with rounded corners](https://github.com/dylanaraps/openbox-patched),
- [Color configuration for i3 online](https://thomashunter.name/i3-configurator/),
- Themes for Openbox: [one](https://github.com/fikriomar16/OBTheme-Collections), [two](https://github.com/addy-dclxvi/openbox-theme-collections)
- [A better Compton from tryone144](https://github.com/tryone144/compton)

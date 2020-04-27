# Apps and Packages
- Necessary: CUPS, GCC, Git, Python, VIM, VIM-X11, XTerm, brightnessctl, ALSA, PulseAudio
- Common choices: Firefox, Chrome, feh, rxvt-unicode, VLC
- My choices: Okular, qpdfview-qt5, Thunar, maim, xclip
- Larger apps: Thunderbird, GIMP, Inkscape, LibreOffice, Telegram Desktop
- UI related: sddm, dmenu, dwm (from this repository), Openbox, tint2, compton
- Necessary for DWM and other 'developer' work: alsa-lib,-devel, libX11,-devel, libXft,-devel, libXinerama,-devel
- For minimal distros: dialog, libinput, iw, wpa\_supplicant, mesa, X11: xorg-server, -server-utils, -xinit, intel-driver, libinput-driver
- Fonts: DejaVu, Terminus, Iosevka, Arimo (some are included in `thicc/fonts`)
- Study related: GNUplot, OpenSSH, ROOT, Ostap
- Other: simplescreenrecorder, klavaro, libinput-gestures

On a new system, tell git who you are, using 
``` bash
git config --global user.name "First name Last name"
git config --global user.email "email@address.domain"
```

# Various Instructions 
## User and Group management
- `useradd -m name` -m created the /home/name directory
- `groupadd name` 
- `gpasswd -a user group` to add and `-d user` to remove. 


## Making xdg-mime respect links
Edit `/usr/bin/xdg-mime` by replacing all 
``` 
mv ${file}.new ${file} 
```
with
``` 
cat ${file}.new > ${file} && rm ${file}.new
```
But this doesn't help all that much, since all applications contain their own copy of the original code with the `mv` command. 

## Fixing hissing sound in headphones
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

## pacman
For installed packages. 
- `pacman -Qi package` for info.
- `pacman -Qs word` to search for the word. 
- `pacman -Qg group` for the group members.
- `pacman -Ql package` for list of files owned by the package. 
- `pacman -Qo file` for the file owner. 
- `pacman -Rs package` to remove the package and all its unneeded dependencies. 
- `pacman -Ru package` to check if the package is needed and if not, then remove. 

For packages from repositories. 
- `pacman -Si package` for info (double `i` sometimes).
- `pacman -Ss word` to search for the word. 
- `pacman -Syyu` to perform a full system update. 
- `pacman -Sc` clean cache with uninstalled packages.

For .pkg.tar.gz files
- `pacman -U file` to install.

In `/etc/pacman.conf`, uncomment the line `#Color` to have colored output. 

## CUPS configuration
1. Create a group named lpadmin.
1. Edit SystemGroup line in /etc/cups/cups-files.conf – add lpadmin. 
1. Add the user to group lpadmin. 
1. Type localhost:631/ in a browser. 
1. Add printer smb://guest@192.168.1.1/Xerox3250. (the format is thing://guest@IP/name)
1. Use driver ph3250.ppd. Set yes in Share checkbox. 

## Deleting commit history in GitHub
``` bash
git checkout --orphan tmp
git add -A
git commit -am "commit"
git branch -D master
git branch -m master
git push -f origin master
```

## Cool dwm patches
Better patches are marked with +, not working with -, very complicated with ?

+-`centeredwindowname`, -`combo`, `fakefullscreen`, `flextile`, +`focusonclick`, `focusurgent`, +?`keymodes`, `noborder`, +`pango`, +`pertag`, ++?`systray`, `tilegap`, `uselessgap`


# Links
- [XBM online editor](https://xbm.jazzychad.net/),
- [Image color picker](https://html-color-codes.info/colors-from-image/),
- [A better image color picker](https://image-color.com/),
- [Openbox-patched with rounded corners](https://github.com/dylanaraps/openbox-patched),
- [Color configuration for i3 online](https://thomashunter.name/i3-configurator/),
- Themes for Openbox: [one](https://github.com/fikriomar16/OBTheme-Collections), [two](https://github.com/addy-dclxvi/openbox-theme-collections)
- [A better Compton from tryone144](https://github.com/tryone144/compton)

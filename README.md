# stuff

Here, I keep all configuration files, which are accessed by apps through symbolic links generated by the [`install.sh`](install.sh) script. You can find what this script can do by running `./install.sh`.

Some features are provided by programs that need to be compiled. `Makefile`s are in [`home/bin`](home/bin/Makefile), [`prj/abduco`](prj/abduco/Makefile), [`prj/dvtm`](prj/dvtm/Makefile), [`prj/stagit`](prj/stagit/Makefile), [`prj/halfmir`](prj/halfmir/Makefile). You may need to edit [`home/bin/batlog.c`](home/bin/batlog.c) and the Makefiles.


## Apps and Packages

- Necessary: Neovim Git GCC Make brightnessctl
- Apps: Alacritty Firefox Chrome Okular Thunderbird GIMP Inkscape LibeOffice Telegram-Desktop VLC simple-scan nnn TexStudio Apostrophe Strawberry simplescreenrecorder klavaro qpdfview-qt5 Thunar
- Utilities: TexLive IPython mksh Rust Cargo Python
- Smaller systems: dmenu openbox tint2 picom feh pavucontrol clipmenu dunst xclip bluez{,-utils} pulseaudio-bluetooth CUPS sane openssh flatpak
- Minimal distros: libinput wpa\_supplicant dhcp mesa-driver xorg{-server,-server-utils,-xinit} intel-driver libinput-driver zip unzip tar pulseaudio
- Developer work: libpulse (pulseaudio-lib), libX11{,-devel} libXft{,-devel} libXinerama{,-devel} libXrandr{,-devel}
- Studies: ROOT, Ostap
- KDE: packagekit{,-qt5} appstream{,-qt5} wl-clipboard latte-dock
- GNOME: dask-to-dock-ext. clipboard-ext. gnome-tweaks

Apps that may appear to be useful in the future:

- guvcview(-qt) -- capture video output in real time and even do something simple to it.
- exiftool -- show and edit files metadata.

The Special directory:
- Import [`okular.shortcuts`](Special/okular.shortcuts) in Okular Settings > Configure Keyboard Shortcuts > Manage Schemes > More Actions > Import Scheme.
- Copy [`30-libinput.conf`](Special/30-libinput.conf) and [`20-display.conf`](Special/20-display.conf) to `/etc/X11/xorg.conf.d/`.
- If Firefox haven't taken care of the bookmarks, import them from [`bookmarks.html`](Special/bookmarks.html).

To use SDDM (or any other display manager)
``` bash
systemctl enable sddm.service
systemctl start sddm.service
systemctl set-default multi-user.target
```

Create an ssh key and add it to GitHub and other services.
``` bash
ssh-keygen -t rsa -b 4096 -C "your_email@example.com"
```

[GitHub](https://docs.github.com/en/github/authenticating-to-github/adding-a-new-ssh-key-to-your-github-account)
[GitLab](https://docs.gitlab.com/ee/ssh/#adding-an-ssh-key-to-your-gitlab-account)
Change all git remotes to ssh `git remote set-url origin ssh:git@[].git`.

DEPRECATED
Create an `optlocal` group, an `/opt/local/` directory with this group and 775 permissions, and add the user to this group.


# Various Instructions
## Backup and restore GNOME configuration
- Backup with `dconf dump / > backup.conf`
- Restore with `dconf load / < backup.conf`

## User and Group management
- `useradd -m name` -m creates the `/home/name` directory.
- `groupadd name`.
- `gpasswd -a user group` to add and `-d user` to remove.
- `chfn -f full_name user` to add a full name for the user.

## Network
From systemd hooks

```
dhcpcd:
    start: dhcpcd -q -w %I
    stop:  dhcpcd -x %I

wpa_supplicant:
    wpa_supplicant -c/etc/wpa_supplicant/wpa_supplicant-%I.conf -i%I -B

NetworkManager:
    nm-online -s -q
```

## Fix hissing sound in headphones
For alsamixer
``` bash
amixer -c PCH cset 'name=Headphone Mic Boost Volume' 1
```
For PulseAudio
``` bash
FILE /usr/share/pulseaudio/alsa-mixer/paths/analog-input-headphone-mic.conf
[Element Headphone Mic Boost]
required-any = any
switch = select
## Replace "volume = merge" by:
volume = 1
override-map.1 = all
override-map.2 = all-left,all-right

FILE /usr/share/pulseaudio/alsa-mixer/paths/analog-input-internal-mic.conf
[Element Headphone Mic Boost]
switch = off
## Replace "volume = off" by:
volume = 1

FILE /etc/pulse/default.pa
# Add tsched=0
load-module module-udev-detect tsched=0
```
And then restart PulseAudio
```
pulseaudio -k
pulseaudio --start
```

See also the [ArchWiki page](https://wiki.archlinux.org/index.php/PulseAudio/Troubleshooting#Static_noise_when_using_headphones).

## pacman
For installed packages.

- `pacman -Qi package` for info.
- `pacman -Qs word` to search for the word.
- `pacman -Qg group` for the group members.
- `pacman -Ql package` for list of files owned by the package.
- `pacman -Qo file` for the file owner.
- `pacman -Qdt` for all unneeded dependency packages.
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

If you have signature trust problems, try updating `gnupg` and then running `pacman-key --refresh-keys`.


## Deleting commit history in GitHub
``` bash
git checkout --orphan tmp
git add -A
git commit -am "commit"
git branch -D master
git branch -m master
git push -f origin master
```

## CUPS configuration
1. Install CUPS package and enable/start org.cups.cupsd.service.
1. Create a group named lpadmin.
1. Edit SystemGroup line in /etc/cups/cups-files.conf – add lpadmin.
1. Add the user to group lpadmin.
1. Type localhost:631/ in a browser.
1. Add samba backend somehow if it is absent.
1. Add printer smb://guest@192.168.1.1/Xerox3250. (the format is thing://guest@IP/name), you can run `smbclient -L 192.168.1.1`.
1. Use driver ph3250.ppd. Set yes in the Share checkbox.

## Thunderbird configuration
CERN email configuration for Thunderbird can be found at [here](https://espace.cern.ch/mmmservices-help/AccessingYourMailbox/Thunderbird/Pages/Configuration-(Thunderbird-31).aspx).

Gmail configuration --- [here](https://support.google.com/mail/answer/78892?hl=en#zippy=%2Cthunderbird).

The font types you need are: Cyrillic, Latin, Other Writing Systems. Set their sizes to 18.

## Making xdg-mime respect links
Edit `/usr/bin/xdg-mime` by replacing all
``` bash
mv ${file}.new ${file}
```
with
``` bash
cat ${file}.new > ${file} && rm ${file}.new
```
But this doesn't help all that much, since all applications contain their own copy of the original code with the `mv` command.

# Links
- [XBM online editor](https://xbm.jazzychad.net/),
- [Image color picker](https://html-color-codes.info/colors-from-image/),
- [A better image color picker](https://image-color.com/),
- [Openbox-patched with rounded corners](https://github.com/dylanaraps/openbox-patched),
- [Color configuration for i3 online](https://thomashunter.name/i3-configurator/),
- Themes for Openbox: [one](https://github.com/fikriomar16/OBTheme-Collections), [two](https://github.com/addy-dclxvi/openbox-theme-collections)
- [A better Compton from tryone144](https://github.com/tryone144/compton)





# stuff
This repo includes all my configuration files for dwm and Openbox; bash, Emacs, VIM, XTerm, URxvt, tint2, Compton dotfiles; Openbox themes and X11 icontheme; KDE shortcuts; TTF fonts. I personally love to keep everything in one place, so I just keep `~/stuff/` directory with all my stuff and create links to files located in it. `install.sh` script does exactly this.


## Installation
All files and directories from this repo except `30-libinput.conf` are placed to home directory (`~/` and `~/.config/`). You can place them automatically using the `install.sh` script:
``` bash
[optionally] cd ~/
git clone https://github.com/ehhov/stuff.git
cd stuff/
./install.sh link
```

`30-libinput.conf` should be copied to `/etc/X11/xorg.conf.d/` manually.


## Small `install.sh` documentation
Obviously, you always can look inside the `install.sh` file yourself, but here is a little description. `install.sh` looks at the first command line argument.
- If it is `rm` or `clean`, the script will remove your own files and directories that replace existing in this repo ones **(regardless of type: rm -rf)**, but will not place new ones.
- If it is `reln` or `relink`, the script will replace your config files (not directories) with links to ones from this repo.
- If it is `ln` or `link`, the script will place links only to these repo files that don't override your own files with the same names.
- If it is `cp` or `copy`, the script will copy only the repo files that don't override your own files with the same names.
- In all other cases the script will print a little help message.

If you don't want to use all the files from this repo, but want to use `install.sh` to handle them, just delete (or rename) the files you don't need and run `install.sh`, it will not cause any errors. 

## dwm
I use [dwm](https://dwm.suckless.org) with a number of patches and custom functions; status information is generated by my own program written in C (its description is in Notes below). To compile everything being in `stuff/` you can type
``` bash
cd dwm/wm/
make
cd ../status/
make
```
But if you haven't changed some paths in `dwm/status/status.c` (especially wifi and brightness), status will not work almost certainly.

The patches in dwm are: autostart (modified), focusonclick (modified: fixed the bug with focus on horizontal scroll), pertag, tilegap, systray. My own patches provide window title alignment control, a way to restore the default layout parameters (mfact, nmaster, layout, gappx, borderpx), a way to change gaps and borders in real time, floating windows borders increment parameter, vertical tiling layout w/ and w/o gaps and monocle layout with gaps (for a similar look everywhere). And I haven't deleted any standard window handling function, all listed are new. 

I don't have the autostart patch because I launch dwm using `~/.xinitrc` file (that's why it is also backed up and is executable). To launch dwm with a display manager, I added an entry to `/usr/share/xsessions/`.


### Notes
Keyboard layouts are defined in `bin/kb`. 

I use tint2 and dwm's status that recieve the outputs of my own programs written in C. These programs are located at `tint2/status/` and `dwm/status`. To compile them you can type (being in `stuff/`)
``` bash
cd dwm/status/
make
```
for dwm and (in `stuff/` again)
``` bash
cd tint2/status/
make
```
for tint2. 

In my configuration `status` prints wifi-info, brightness-info, sound-info, keyboard-layout-info, battery-info, time. The look of status info is obviously different for dwm and tint2. wifi-info shows the name of the network you are connected to and download & upload speeds, brightness-info shows is brightness max, sound-info shows speaker icons in tint2 and is\_muted:percent in dwm (instantly updates on any action upon sound, including headphones plug in), keyboard-layout-info shows the current layout (updates on layout change instantly), battery-info shows percent, is\_changing, time left, and time shows weekday, month, day in month, time.

I also recommend looking through these programs (only `status.c` files), if you want to use them, but it may take a while. Almost certainly you have other paths for wifi statistics and brightness information and other keyboard layouts. All these parameters are located at the top of each program.

**Dependencies** are: pulseaudio-libs-dev, libX11-dev, iw.

Brighness changer script `bin/brightness` by default uses a step of 2%, but can provide dynamic brightness step. To use it you should change brightness path in `bin/brightness-levels.c` file and compile it (being in `bin/`) `gcc brightness-levels.c -o brightness-levels` or `make`.
If dynamic step works, you will have faster brightness change at high brightness and far more subtle adjustment at low brightness (compared to the constant step of 2%).

`PS1` variable in my `bashrc` contains a program that prints time, I don't use `date +...%_I...` command because it has an additional space for one-digit hours. So you have to either edit `PS1` or compile `bash-time.c` in `bin/` using `gcc bash-time.c -o bash-time` or `make`. 

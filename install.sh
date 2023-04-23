#!/bin/sh

dir="$(readlink -f "$0")"
dir="$(dirname "$dir")"
rel="${dir#~/}"


create_link() {
	# link to_what where
	if [ ! -e "$2" ] && [ ! -h "$2" ]; then
		ln -s "$1" "$2"
	fi
}


remove() {
	# remove what
	if [ -e "$1" ] || [ -h "$1" ]; then
		rm -rf "$1"
	fi
}


case $1 in
	re*) action() { remove "$2"; create_link "$1" "$2"; };;
	rm*|remove) action() { remove "$2"; };;
	*)   action() { create_link "$1" "$2"; };;
esac


case $1 in
ln|link|reln|relink|rm|remove)
	for i in "${dir}"/home/*; do
		i="${i#$dir/}"
		j="${i#home/}"
		action "${rel}/$i" ~/."$j"
	done
	for i in "${dir}"/config/*; do
		i="${i#$dir/}"
		j="${i#config/}"
		action ../"${rel}/$i" ~/.config/"$j"
	done
;;

kde*|rekde*|rmkde*)
	case $1 in
	*w*) suffix=wayland;;
	*x*) suffix=x11;;
	*) echo 'X11 or Wayland?'; exit;;
	esac
	for i in "${dir}"/KDE/config-$suffix/*; do
		i="${i#$dir/}"
		j="${i#KDE/config-$suffix/}"
		action ../"${rel}/$i" ~/.config/"$j"
	done
	for i in "${dir}"/KDE/share-$suffix/*; do
		i="${i#$dir/}"
		j="${i#KDE/share-$suffix/}"
		action ../../"${rel}/$i" ~/.local/share/"$j"
	done
;;

sw*kde*)
	case $1 in
	sw*w*)
		$0 rmkde-x11
		$0 rekde-wayland
	;;
	sw*x*)
		$0 rmkde-wayland
		$0 rekde-x11
	;;
	*)
		echo 'X11 or Wayland?'
	;;
	esac
;;

# WARNING: may not work with per-desktop env. configurations
#mime)
#	file=~/.config/mimeapps.list
#	here="${dir}"/config/mimeapps.list
#	if [ -f "$file" ] && [ ! -h "$file" ]; then
#		cat "$file" >| "$here"
#	fi
#	here="${here#$dir/}"
#	remove "$file"
#	create_link ../"${rel}/$here" "$file"
#;;

gits)
	cd ~
	[ -e thicc ] || git clone https://github.com/ehhov/thicc.git
	[ -e work ] || git clone https://github.com/ehhov/work.git
	[ -e workcern ] || git clone https://gitlab.cern.ch/kerimgus/workcern.git
	mkdir -p ~/gits
	cd ~/gits
	[ -e dwm ] || git clone https://github.com/ehhov/dwm.git
	[ -e st ] || git clone https://github.com/ehhov/st.git
	[ -e status ] || git clone https://github.com/ehhov/status.git
	[ -e xshot ] || git clone https://github.com/ehhov/xshot.git
	[ -e xpick ] || git clone https://github.com/ehhov/xpick.git
	[ -e emonc ] || git clone https://github.com/ehhov/emonc.git
	[ -e filemanager ] || git clone https://github.com/ehhov/filemanager.git
	[ -e forest ] || git clone https://github.com/ehhov/forest.git
;;

*)
	echo -e "$0: usage ('|' means 'or', [] means optional) :"
	echo -e "  $0 ln|link      to create links"
	echo -e "  $0 reln|relink  to remove your files and create links"
	echo -e "  $0 rm|remove    to remove the files (whether they are links from here or not)"
	echo -e "  $0 [re]kde      to \`[re]link\` files for KDE. X11 or Wayland"
	echo -e "  $0 rmkde        to \`remove\` files for KDE. X11 or Wayland"
	echo -e "  $0 sw*kde       to switch from one KDE configuration to another. X11 or Wayland"
	#echo -e "  $0 mime         to copy ~/.config/mimeapps.list to here if it is a regular file"
	echo -e "  $0 gits         to clone all the gits"
	echo -e ""
;;


esac

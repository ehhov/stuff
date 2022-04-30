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
	*)   action() { create_link "$1" "$2"; };;
esac

case $1 in
ln|link|reln|relink)
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

rm|remove)
	for i in "${dir}"/home/*; do
		i="${i#$dir/}"
		j="${i#home/}"
		remove ~/."$j"
	done
	for i in "${dir}"/config/*; do
		i="${i#$dir/}"
		j="${i#config/}"
		remove ~/.config/"$j"
	done
;;

kde|rekde)
	for i in "${dir}"/KDE/config/*; do
		i="${i#$dir/}"
		j="${i#KDE/config/}"
		action ../"${rel}/$i" ~/.config/"$j"
	done
	for i in "${dir}"/KDE/share/*; do
		i="${i#$dir/}"
		j="${i#KDE/share/}"
		action ../../"${rel}/$i" ~/.local/share/"$j"
	done
;;

rmkde)
	for i in "${dir}"/KDE/config/*; do
		i="${i#$dir/}"
		j="${i#KDE/config/}"
		remove ~/.config/"$j"
	done
	for i in "${dir}"/KDE/share/*; do
		i="${i#$dir/}"
		j="${i#KDE/share/}"
		remove ~/.local/share/"$j"
	done
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
;;

*)
	echo -e "$0: usage ('|' means 'or', [] means optional) :"
	echo -e "  $0 ln|link      to create links"
	echo -e "  $0 reln|relink  to remove your files and create links"
	echo -e "  $0 rm|remove    to remove the files (whether they are links from here or not)"
	echo -e "  $0 [re]kde      to \`[re]link\` files for KDE"
	echo -e "  $0 rmkde        to \`remove\` files for KDE"
	#echo -e "  $0 mime         to copy ~/.config/mimeapps.list to here if it is a regular file"
	echo -e "  $0 gits         to clone all the gits"
	echo -e ""
;;


esac

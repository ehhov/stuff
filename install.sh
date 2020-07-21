#!/bin/sh

dir=$(readlink -f $0)
dir=$(dirname "$dir")
rel=${dir#~/}
create_link() {
	# link to_what where
	if [ ! -e $2 ] && [ ! -L $2 ]; then
		ln -s $1 $2
	fi
}
copy() {
	# copy what where
	if [ ! -e $2 ] && [ ! -L $2 ]; then
		cp $1 $2
	fi
}
remove() {
	# remove what
	if [ -e $1 ] || [ -L $1 ]; then
		rm -rf $1
	fi
}


case $1 in
ln|link)
	for i in ${dir}/home/*; do
		i=${i#$dir/}
		j=${i#home/}
		create_link ${rel}/$i ~/.$j
	done
	for i in ${dir}/config/*; do
		i=${i#$dir/}
		j=${i#config/}
		create_link ../${rel}/$i ~/.config/$j
	done
;;

reln|relink)
	for i in ${dir}/home/*; do
		i=${i#$dir/}
		j=${i#home/}
		remove ~/.$j
		create_link ${rel}/$i ~/.$j
	done
	for i in ${dir}/config/*; do
		i=${i#$dir/}
		j=${i#config/}
		remove ~/.config/$j
		create_link ../${rel}/$i ~/.config/$j
	done
;;

rm|remove)
	for i in ${dir}/home/*; do
		i=${i#$dir/}
		j=${i#home/}
		remove ~/.$j
	done
	for i in ${dir}/config/*; do
		i=${i#$dir/}
		j=${i#config/}
		remove ~/.config/$j
	done
;;

kde|KDE)
	for i in ${dir}/KDE/*; do
		i=${i#$dir/}
		j=${i#KDE/}
		remove ~/.config/$j
		create_link ../${rel}/$i ~/.config/$j
	done
;;

mime)
	file=~/.config/mimeapps.list
	here=${dir}/config/mimeapps.list
	if [ -f $file ] && [ ! -L $file ]; then
		cp $file $here
	fi
	here=${here#$dir/}
	remove $file
	create_link ../${rel}/$here $file
;;

clone)
	cd ~
	[ -e thicc ] || git clone https://github.com/ehhov/thicc.git
	mkdir -p ~/gits
	cd ~/gits
	[ -e status ] || git clone https://github.com/ehhov/status.git
	[ -e st ] || git clone https://github.com/ehhov/st.git
	[ -e dwm ] || git clone https://github.com/ehhov/dwm.git
	[ -e xshot ] || git clone https://github.com/ehhov/xshot.git
	[ -e xpick ] || git clone https://github.com/ehhov/xpick.git
	[ -e emonc ] || git clone https://github.com/ehhov/emonc.git
;;

*)
	echo -e "$0: usage ('|' means 'or') :"
	echo -e "  $0 ln|link      to create links"
	echo -e "  $0 reln|relink  to remove your files and create links"
	echo -e "  $0 rm|remove    to remove the files (whether they are installed or not)"
	echo -e "  $0 kde          to \`relink\` files for KDE"
	echo -e "  $0 mime         to copy ~/.config/mimeapps.list to here if it is a regular file"
	echo -e ""
;;


esac


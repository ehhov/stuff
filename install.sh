#! /bin/bash

chmod +x to_home/bin/*
chmod -x to_home/bin/*.c
chmod -x to_home/bin/Makefile

this=$(pwd)
this=${this#~/}
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
fix_mime() {
	# copy the new mimeapps if it has been changed (and hence is now a regular file)
	file=~/.config/mimeapps.list
	here=to_config/mimeapps.list
	if [ -f $file ] && [ ! -L $file ]; then 
		cp $file $here
	fi
}


# every time take account of the inadequacy of xdg-mime 
#fix_mime

case $1 in
ln|link)
	for i in to_home/*; do 
		j=${i#to_home/}
		create_link ${this}/$i ~/.$j
	done
	for i in to_config/*; do
		j=${i#to_config/}
		create_link ../${this}/$i ~/.config/$j
	done
;;

reln|relink)
	for i in to_home/*; do 
		j=${i#to_home/}
		remove ~/.$j
		create_link ${this}/$i ~/.$j
	done
	for i in to_config/*; do
		j=${i#to_config/}
		remove ~/.config/$j
		create_link ../${this}/$i ~/.config/$j
	done
;;

kde|KDE)
	for i in KDE/*; do
		j=${i#KDE/}
		remove ~/.config/$j
		create_link ../${this}/$i ~/.config/$j
	done
;;

*)
	echo -e "$0: usage ('|' means 'or') :"
	echo -e "  $0 ln|link \t\t to create links"
	echo -e "  $0 reln|relink \t to remove your files and create links"
	echo -e "  $0 kde \t\t to \`relink\` files for KDE"
	echo -e ""
;;


esac


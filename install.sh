#! /bin/bash

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


case $1 in
ln|link)
	for i in home/*; do 
		j=${i#home/}
		create_link ${this}/$i ~/.$j
	done
	for i in config/*; do
		j=${i#config/}
		create_link ../${this}/$i ~/.config/$j
	done
;;

reln|relink)
	for i in home/*; do 
		j=${i#home/}
		remove ~/.$j
		create_link ${this}/$i ~/.$j
	done
	for i in config/*; do
		j=${i#config/}
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

mime)
	file=~/.config/mimeapps.list
	here=config/mimeapps.list
	if [ -f $file ] && [ ! -L $file ]; then 
		cp $file $here
	fi
	remove $file
	create_link ../${this}/$here $file
;;

*)
	echo -e "$0: usage ('|' means 'or') :"
	echo -e "  $0 ln|link \t\t to create links"
	echo -e "  $0 reln|relink \t to remove your files and create links"
	echo -e "  $0 kde \t\t to \`relink\` files for KDE"
	echo -e "  $0 mime \t\t to copy ~/.config/mimeapps.list to here if it is a regular file"
	echo -e ""
;;


esac


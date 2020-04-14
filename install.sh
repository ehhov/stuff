#! /bin/bash

chmod +x to_home/bin/*
chmod -x to_home/bin/*.c
chmod -x to_home/bin/Makefile

Ln_to_home='bash_profile bashrc emacs i3 vimrc Xresources themes bin dwm st'
Ln_to_config='fontconfig libinput-gestures.conf openbox tint2 compton mimeapps.list'
Ln_KDE='kglobalshortcutsrc khotkeysrc'


yes='Yes, delete all my own configuration files and folders (rm -rf) that can be replaced with ones from this repo'
no='No, leave my configuration files alive'
PS3='Are you sure? (1/2): '

create_link() {
	# create what where
	if [ ! -e $2 ] && [ ! -L $2 ] && [ -e $1 ]; then  
		ln -s $1 $2
	fi
}
copy() {
	# copy what where
	if [ ! -e $2 ] && [ ! -L $2 ] && [ -e $1 ]; then  
		cp $1 $2
	fi
}
remove() {
	# remove what
	if [ -e $1 ] || [ -L $1 ]; then 
		rm $1
	fi
}

case $1 in
	ln|link)
	for i in to_home/*; do 
		j=${i#to_home/}
		create_link $(pwd)/$i ~/.$j
	done
	for i in to_config/*; do
		j=${i#to_config/}
		create_link $(pwd)/$i ~/.config/$j
	done
	for i in KDE/*; do
		j=${i#KDE/}
		create_link $(pwd)/$i ~/.config/$j
	done
	;;

	reln|relink)
	for i in to_home/*; do 
		j=${i#to_home/}
		remove ~/.$j
		create_link $(pwd)/$i ~/.$j
	done
	for i in to_config/*; do
		j=${i#to_config/}
		remove ~/.config/$j
		create_link $(pwd)/$i ~/.config/$j
	done
	for i in KDE/*; do
		j=${i#KDE/}
		remove ~/.config/$j
		create_link $(pwd)/$i ~/.config/$j
	done
	;;

	cp|copy)
	for i in to_home/*; do 
		j=${i#to_home/}
		copy $(pwd)/$i ~/.$j
	done
	for i in to_config/*; do
		j=${i#to_config/}
		copy $(pwd)/$i ~/.config/$j
	done
	for i in KDE/*; do
		j=${i#KDE/}
		copy $(pwd)/$i ~/.config/$j
	done
	;;

	rm|clean)
	select j in "$yes" "$no"
	do
		case $j in 
			$yes)
				echo 'Deleting...'
				for i in to_home/*; do 
					j=${i#to_home/}
					remove ~/.$j
				done
				for i in to_config/*; do
					j=${i#to_config/}
					remove ~/.config/$j
				done
				for i in KDE/*; do
					j=${i#KDE/}
					remove ~/.config/$j
				done
				break ;;
			$no)
				echo 'You saved them'
				break ;;
		esac
	done
	;;

	*)
	echo -e "$0: usage ('|' means 'or') :"
	echo -e "  $0 ln|link \t\t to create links"
	echo -e "  $0 reln|relink \t to remove your files and create links"
	echo -e "  $0 cp|copy \t\t to copy files instead of creating links"
	echo -e "  $0 rm|clean \t to remove your files"
	echo -e ""
	;;


esac


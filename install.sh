#! /bin/bash

chmod +x ./bin/*
chmod -x ./bin/*.c
chmod -x ./bin/Makefile

D=`pwd`
Ln_to_home='bash_profile bashrc emacs i3 vimrc Xresources themes bin dwm st'
Ln_to_config='fontconfig libinput-gestures.conf openbox tint2 compton'
Ln_KDE='kglobalshortcutsrc khotkeysrc'


yes='Yes, delete all my own configuration files and folders (rm -rf) that can be replaced with ones from this repo'
no='No, leave my configuration files alive'
PS3='Are you sure? (1/2): '


case $1 in
	relink|reln)
	for i in $Ln_to_home
	do 
		[ -e $D/$i ] && { [ -e ~/.$i ] && rm ~/.$i; ln -s $D/$i ~/.$i; }
	done
	for i in $Ln_to_config
	do
		[ -e $D/$i ] && { [ -e ~/.config/$i ] && rm ~/.config/$i; ln -s $D/$i ~/.config/$i; }
	done
	for i in $Ln_KDE
	do
		[ -e $D/KDE/$i ] && { [ -e ~/.config/$i ] && rm ~/.config/$i; ln -s $D/KDE/$i ~/.config/$i; }
	done
	;;

	rm|clean)
	select j in "$yes" "$no"
	do
		case $j in 
			$yes)
				echo 'Deleting...'
				for i in $Ln_to_home
				do 
					[ -e $D/$i -a -e ~/.$i ] && rm -rf ~/.$i
				done
				for i in $Ln_to_config
				do
					[ -e $D/$i -a -e ~/.config/$i ] && rm -rf ~/.config/$i
				done
				for i in $Ln_KDE
				do 
					[ -e $D/KDE/$i -a -e ~/.config/$i ] && rm -rf ~/.config/$i
				done
				break ;;
			$no)
				echo 'You saved them'
				break ;;
		esac
	done
	;;

	mv|move)
	for i in $Ln_to_home
	do 
		[ ! -e ~/.$i -a -e $D/$i ] && mv $D/$i ~/.$i
	done
	for i in $Ln_to_config
	do
		[ ! -e ~/.config/$i -a -e $D/$i ] && mv $D/$i ~/.config/$i
	done
	for i in $Ln_KDE
	do 
		[ ! -e ~/.config/$i -a -e $D/KDE/$i ] && mv $D/KDE/$i ~/.config/$i
	done
	;;

	ln|link)
	for i in $Ln_to_home
	do 
		[ ! -e ~/.$i -a -e $D/$i ] && ln -s $D/$i ~/.$i
	done
	for i in $Ln_to_config
	do
		[ ! -e ~/.config/$i -a -e $D/$i ] && ln -s $D/$i ~/.config/$i
	done
	for i in $Ln_KDE
	do 
		[ ! -e ~/.config/$i -a -e $D/KDE/$i ] && ln -s $D/KDE/$i ~/.config/$i
	done
	;;

	*)
	echo -e "$0: usage ('|' means 'or') :"
	echo -e "  $0 ln|link \t\t to create links"
	echo -e "  $0 reln|relink \t to remove your files and create links"
	echo -e "  $0 rm|clear|clean \t to remove your files"
	echo -e "  $0 mv|move \t\t to move files instead of creating links"
	echo -e ""
	;;


esac


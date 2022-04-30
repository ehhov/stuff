#!/bin/sh
set -- mimeapps.list dconf gtk-3.0

config=${XDG_CONFIG_HOME:-~/.config}
session=${XDG_CURRENT_DESKTOP:-$XDG_SESSION_DESKTOP}

case $session in
*kde*|*KDE*|*plasma*|*Plasma*)
	session=kde
	;;
*gnome*|*GNOME*)
	session=gnome
	;;
*)
	session=other
	;;
esac

for file in "$@"; do
	if [ -e "$config/$file-$session" ]; then
		rm -rf "$config/$file"
		ln -s "$file-$session" "$config/$file"
	fi
done


# This could go in ~/.config/autostart
# [Desktop Entry]
# Type=Application
# Name=Per DE Config Files
# Exec=sh -c "~/.bin/per-de-config.sh"

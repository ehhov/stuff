#
# ~/.bash_profile
#

#[[ -f ~/.bashrc ]] && . ~/.bashrc

# .bash_profile
#
#  NOTE: This file is for common actions only, and should not be 
#  modified by the user.  User additions should go into the file 
#  "local.profile" in your home area.
#
# Source local definitions

if [ -f /etc/bashrc ]; then
		. "/etc/bashrc"
fi

if [ "$PS1" ]; then
#  PS1="[\h]:\w > "
  PS1="[\u \W]$ "
fi

if [ "$PS1" ]; then
# ignore ^d
#    set -o ignoreeof
    set -o noclobber
    export IGNOREEOF=2
#
  stty stop ^S
  stty intr ^C
#  stty erase ^H
fi

# User specific environment and startup programs
#
# set the prompts

PAGER=less; export PAGER    # Define the default pager for the man command.
EDITOR=vim; export EDITOR # Define the default editor

# additional local strange things.

if [  -f "local.profile" ]; then 
  . "local.profile"
fi

# Normal local thing
if [ -f ~/.bashrc ]; then
  . ~/.bashrc
fi



PATH=/bin:/sbin:/usr/bin:/usr/sbin:/usr/X11R6/bin:/usr/local/bin:/usr/local/sbin:${PATH}":${HOME}/bin";export PATH

MANPATH=${MANPATH}":/usr/share/man"; export MANPATH

date

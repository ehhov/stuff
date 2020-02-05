#
# ~/.bashrc
#

# If not running interactively, don't do anything
[[ $- != *i* ]] && return

#alias ls='ls --color=auto'
#PS1='[\u@\h \W]\$ '

# .bashrc
#
# User specific aliases and functions
#
# Source global definitions

if [  -f "local.shrc" ]; then 
    . "local.shrc"
fi


if [ "$PS1" ]; then
	#PS1="[\u \w]$ "
  #PS1='\n┬─ \u \w [$(date +%l:%M:%S\ %P)]\n└─> '
  #PS1='[\u][\w] [$(date +%l:%M:%S\ %P)]\n → '
  #PS1=' \[\e[34m\]$(date +%l:%M:%S%P) \[\e[35m\]\u \[\e[32m\]\w \[\e[33m\] → \[\e[m\]'
  #PS1=' \[\e[33m\]+ \[\e[34m\]$(date +%l:%M:%S%P) \[\e[35m\]\u \[\e[32m\]\w \[\e[33m\]→ \[\e[m\] '
  PS1=' \[\e[33m\]+ \[\e[34m\]$(~/.bin/bash-time) \[\e[35m\]\u \[\e[32m\]\w \[\e[33m\]→ \[\e[m\] '
fi


# My own willings

alias ls='ls --color=auto'
alias python='python3'
alias pdflatex='pdflatex -file-line-error -interaction=nonstopmode'
export EDITOR=vim
export DE=KDE
#export KDE_FULL_SESSION=true


export TeXauxiliary='aux bbl blg out toc log synctex.gz nav snm lof lot bit idx glo bcf ilg ind fdb_latexmk fls'
texclear() {
	for file in $@
	do
		file=${file%.tex}
		file=${file%.}
    for i in ${TeXauxiliary}
    do
       if test -f $file.$i; then rm $file.$i; fi
    done
	done
}
export -f texclear

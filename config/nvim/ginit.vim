set guioptions+=cemgp
set guifont=Monospace:h12

if exists('g:GuiLoaded')
	GuiTabline 0
	GuiPopupmenu 0
	call GuiMousehide(1)
endif

set bg=light
colorscheme neosolarized
colorscheme gui_github

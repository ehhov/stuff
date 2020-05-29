set guioptions+=cemgrlbp
set guifont=Monospace:h10
GuiTabline 0
GuiPopupmenu 0
call GuiMousehide(1)

set bg=dark
colorscheme neosolarized





let s:TermGUIPrefix = "/home/kerim/.Xresources.d/term_"

fun s:SetTermGUIColorScheme(name)
	let l:thename = s:TermGUIPrefix . a:name . ".vim"
	if filereadable(l:thename)
		exe "source " . l:thename
	endif
endfun

command -nargs=1 Colorscheme call s:SetTermGUIColorScheme(<f-args>)

"call s:SetTermGUIColorScheme("light")


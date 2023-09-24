command -buffer UseCustomIndent setl indentexpr=GetCustomTeXIndent(v:lnum)
command -buffer DontUseCustomIndent setl indentexpr=GetTeXIndent()


let s:maxoff = 50
let s:timeout = 250


fun! s:SearchBracket(fromlnum, flags)
	return searchpairpos('[[({]', '', '[])}]', a:flags,
	\ {-> synstack('.', col('.'))
	\ ->indexof({_, id -> synIDattr(id, 'name') =~ '\%(Comment\|Todo\|String\)$'}) >= 0},
	\ [0, a:fromlnum - s:maxoff]->max(), s:timeout)
endfun


fun! GetCustomTeXIndent(lnum)
	call cursor(a:lnum, 1)
	let l:prev = prevnonblank(a:lnum - 1)
	let [l:br_l, l:br_c] = s:SearchBracket(l:prev, 'nbW')
	if l:br_l > 0 && l:br_c != col([l:br_l, '$']) - 1
		return l:br_c
	endif

	call cursor(l:prev, 1)
	let l:prev = prevnonblank(l:prev - 1)
	let [l:br_l, l:br_c] = s:SearchBracket(l:prev, 'nbW')
	call cursor(a:lnum, 1)
	if l:br_l > 0 && l:br_c != col([l:br_l, '$']) - 1
		return GetTeXIndent() - l:br_c + indent(l:br_l)
	endif

	return GetTeXIndent()
endfun

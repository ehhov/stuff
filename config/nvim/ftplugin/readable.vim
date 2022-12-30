setl spell

setl tw=72 ts=2 sts=0 sw=0 et nosta

setl fo+=taw

fun! Togfo()
	if &fo =~ 'a'
		setl fo-=t
		setl fo-=a
	else
		setl fo+=ta
	endif
endfun

inoremap <buffer>  <c-y>  <cmd>call Togfo()<cr>

"aug text_formatting
"	au!
"	au InsertEnter <buffer>
"		\ exe 'set fo+=t' |
"		\ exe 'set fo+=a' |
"aug END

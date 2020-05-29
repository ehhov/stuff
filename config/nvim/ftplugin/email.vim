setl spell 

setl tw=72 ts=2 sts=2 sw=2 et nosta 

setl fo-=t
setl fo-=a

inoremap <buffer> <c-y> <c-o>:set fo+=ta <cr>i<bs>

aug email_formatting
	au! 
	au InsertLeave <buffer>
		\ exe 'set fo-=t' | 
		\ exe 'set fo-=a' | 
aug END

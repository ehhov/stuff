setl spell

setl tw=72 ts=2 sts=2 sw=2 et nosta

setl fo+=ta

inoremap <buffer> <c-y> <c-o>:set fo-=ta<cr>

aug email_formatting
	au!
	au InsertEnter <buffer>
		\ exe 'set fo+=t' |
		\ exe 'set fo+=a' |
aug END

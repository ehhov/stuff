set buftype=nofile
set scrolloff=9999

fun! s:percent()
	if v:count == 0
		echo 'Count required'
	else
		call cursor(line('$') * v:count / 100, 0)
	endif
endfun

nnoremap <nowait> <buffer>  j  <c-e>
nnoremap <nowait> <buffer>  k  <c-y>
nnoremap <nowait> <buffer>  e  <c-e>
nnoremap <nowait> <buffer>  y  <c-y>
nnoremap <nowait> <buffer>  d  <c-d>
nnoremap <nowait> <buffer>  u  <c-u>
nnoremap <nowait> <buffer>  f  <c-f>
nnoremap <nowait> <buffer>  b  <c-b>
nnoremap <nowait> <buffer>  p  <cmd>call <sid>percent()<cr>
nnoremap <nowait> <buffer>  %  <cmd>call <sid>percent()<cr>
nnoremap <nowait> <buffer>  g  g
nnoremap <nowait> <buffer>  q  <cmd>quit<cr>

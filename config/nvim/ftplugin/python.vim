" fixes extra large tabs proposed by pip
" let g:python_recommended_style = 0
setl ts=4 sts=0 sw=0 et

command! -buffer -nargs=? R  update <bar> ! python3 % <args>
command! -buffer -nargs=? R2 update <bar> ! python2 % <args>
command! -buffer -nargs=? O  update <bar> ! ostap -qb % <args>
command! -buffer -nargs=? CheckSyntax update <bar> ! python3 -m py_compile % <args>
command! -buffer -nargs=? PEP update <bar> ! pycodestyle % <args>

command! -buffer -nargs=0 CC if &l:cc != 80 | exe 'setl cc=80' | else | exe 'setl cc=' | endif

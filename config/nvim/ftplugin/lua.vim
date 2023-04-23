command! -buffer  R  update <bar> source %
command! -buffer -nargs=?  Rlua  update <bar> ! lua % <args>

" hi luaError cterm=reverse ctermfg=6 ctermbg=none

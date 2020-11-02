"let g:rust_recommended_style = 0

" Dynamic is faster and this is the only thing I need here.
command! -buffer -nargs=? R update <bar> ! rustc -C prefer-dynamic % -o /tmp/a && exec /tmp/a

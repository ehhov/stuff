" fixes extra large tabs proposed by pip
let g:python_recommended_style = 0

command! -buffer -nargs=? R  update <bar> ! python3 % <args>
command! -buffer -nargs=? R2 update <bar> ! python2 % <args>
command! -buffer -nargs=? O  update <bar> ! ostap -qb % <args>

" fixes extra large tabs proposed by pip
let g:python_recommended_style = 0

command! -buffer -nargs=? R  update <bar> ! exec python3 % <args>
command! -buffer -nargs=? R2 update <bar> ! exec python2 % <args>
command! -buffer -nargs=? O  update <bar> ! exec ostap -qb % <args>

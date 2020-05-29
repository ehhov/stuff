" fixes extra large tabs proposed by pip
let g:python_recommended_style = 0

command! -buffer -nargs=? R  w <bar> ! python3 % <args> 
command! -buffer -nargs=? R2 w <bar> ! python2 % <args> 

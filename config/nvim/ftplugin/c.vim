set path+=/usr/include/**

if exists("b:is_cpp")
 finish
endif

command! -buffer -nargs=? Rarg update <bar> ! gcc % -o /tmp/a && /tmp/a <args>
command! -buffer -nargs=? R update <bar> ! gcc <args> % -o /tmp/a && /tmp/a

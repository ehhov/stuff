set path+=/usr/include/**

if exists("b:is_cpp")
 finish
endif

command! -buffer -nargs=? R  update <bar> ! gcc % -o /tmp/a && exec /tmp/a <args>
command! -buffer -nargs=? Rl update <bar> ! gcc <args> % -o /tmp/a && exec /tmp/a

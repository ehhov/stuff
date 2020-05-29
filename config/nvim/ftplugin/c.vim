if exists("b:is_cpp")
 finish 
endif

command! -buffer -nargs=? Rarg w <bar> ! gcc % -o /tmp/a && /tmp/a <args>
command! -buffer -nargs=? R w <bar> ! gcc <args> % -o /tmp/a && /tmp/a

let b:is_cpp = 1

command! -buffer -nargs=? Rarg w <bar> ! g++ % -o /tmp/a && /tmp/a <args>
command! -buffer -nargs=? R w <bar> ! g++ <args> % -o /tmp/a && /tmp/a

let b:is_cpp = 1

command! -buffer -nargs=? R  update <bar> ! g++ % -o /tmp/a && exec /tmp/a <args>
command! -buffer -nargs=? Rl update <bar> ! g++ <args> % -o /tmp/a && exec /tmp/a

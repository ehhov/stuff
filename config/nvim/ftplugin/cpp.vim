let b:is_cpp = 1

command! -buffer -nargs=? Rarg update <bar> ! g++ % -o /tmp/a && /tmp/a <args>
command! -buffer -nargs=? R update <bar> ! g++ <args> % -o /tmp/a && /tmp/a

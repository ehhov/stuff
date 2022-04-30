command! -buffer -nargs=? S update <bar> ! source %:p <args>
command! -buffer -nargs=? R update <bar> ! %:p <args>
command! -buffer -nargs=? Chmod update <bar> exe '! chmod +x %' <bar> edit

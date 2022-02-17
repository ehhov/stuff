command! -buffer -nargs=? R update <bar> ! source %:p <args>
command! -buffer -nargs=? Run update <bar> ! %:p <args>
command! -buffer -nargs=? Chmod update <bar> ! chmod +x %

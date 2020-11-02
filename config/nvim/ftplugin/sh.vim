command! -buffer -nargs=? R update <bar> ! exec source %:p <args>
command! -buffer -nargs=? Run update <bar> ! exec %:p <args>

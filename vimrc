set nocompatible " not compatible with the older vi

set number	   " set line numbers
set mouse=a    " ordinary mouse settings
set autochdir  " automatically change directory to the file opened
set history=50 " keep 50 lines of command line history
set showcmd    " display incomplete commands
set dictionary=/usr/share/dict/words

" Set the same indentation settings for all filetypes
filetype indent plugin off
set tabstop=2
set softtabstop=2
set shiftwidth=2
set autoindent 

" Dot, Do execute a current file in the shell
" Dir, D go to the directory, where a current file is located
" C, CC compile and execute
" Py execute 
command Dot w | ! . %
command Do  w | ! chmod +x %:p; %:p
command Dir e %:p:h
command D   e %:p:h
command Xx  ! mimeopen %:p
command -nargs=? C  w | ! gcc % -o /tmp/a && /tmp/a <args>
command -nargs=? CC w | ! g++ % -o /tmp/a && /tmp/a <args>
command -nargs=? M  wa| ! make <args>
command -nargs=? Py w | ! python3 % <args>
command -nargs=? IPy w| ! ipython3 % <args>
command -nargs=? ROOT   w | ! . ~/work/todot ; root -b -q %<args>
command -nargs=? PyROOT w | ! . ~/work/todot ; ipython2 %<args>
command -nargs=? Tex w | ! pdflatex -file-line-error -interaction=nonstopmode % <args>
command -nargs=? Xetex w|! xelatex % <args>
command -nargs=? Okular ! okular %:r.pdf & disown

set laststatus=2 "increase the number of lines at the bottom to place info
set statusline=%F\ %m  " display filename and is edited/editable 
set statusline+=%=     " go to the other side of bar
set statusline+=line\ %l,\ col\ %c\ %4P\  " col, line, percent of file length

vmap <C-c> "+y
vmap <C-x> "+c
"nmap <C-v> "+p
imap <C-v> <Esc>"+pa

"let g:netrw_browsex_viewer="mimeopen"
let g:netrw_browse_split = 3
let g:netrw_liststyle = 1
let g:netrw_sizestyle = "h"
autocmd filetype netrw nnoremap <buffer> O :! okular %:p:h/<cfile> & <enter>

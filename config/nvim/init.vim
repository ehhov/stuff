" https://github.com/neovim/neovim/issues/8715
set mouse=a
set number
set cursorline
set history=50
set showcmd
set whichwrap+=[,]  " wrap lines with arrow keys in insert mode
set scrolloff=2
set fillchars=fold:-
set title

set hlsearch
set noincsearch
set ignorecase
set smartcase
set virtualedit=block
set noshowmatch

set autochdir
set autoread
set nohidden
set wildmenu
set complete+=kspell,i
set path+=**
set shortmess-=F

set guicursor=n-v-c-sm:block,i-ci-ve:ver25,r-cr-o:hor20
set guioptions+=cemgp
set guifont=monospace:h9

set wrap
set linebreak
set breakindent
set showbreak=\|

set tabstop=8
set softtabstop=0
set shiftwidth=0
set smarttab
set autoindent
set copyindent
set preserveindent

set foldmethod=marker
filetype on
syntax on
filetype plugin on
filetype indent on
set formatoptions=croqj1
" format comments; add comment leader on <cr> and o; format with gq;
" (not w) use trailing space to indicate paragraph continuation;
" remove comment splitting wisely; (not n) format numbered lists;
" don't split lines at single letter words
set nojoinspaces

set laststatus=2
let &statusline = '%#StatusLineNC# %F %m%r%h%w %=%* %-5k%-16(%l,%c%V / %LL%) %P '
"let &statusline = '%='
"set fillchars+=stl:─,stlnc:─

command! EE update <bar> e %
command! Syn let &ft = &ft
command! -nargs=? -complete=file M  update <bar> ! make <args>
command! -nargs=? -complete=file Mw  wa <bar> ! make <args>
command! -nargs=? Count keeppatterns exe '%s/'.<q-args>.'//gn' | nohlsearch
command! -nargs=1 HLSearch exe 'set hlsearch' | let @/ = <q-args>
command! -nargs=1 -complete=file Open update <bar> ! setsid -f xdg-open <q-args> </dev/null >/dev/null 2>&1
command! -nargs=1 Readable if <q-args> == 'yes' | runtime ftplugin/readable.vim | else | echo 'Yes?' | endif
command! Terminal tabnew <bar> terminal
command! Notepad below new | resize 5 | setl winfixheight buftype=nofile
command! Justify exe 'normal! i'.repeat(' ', &tw - len(getline('.'))).'<esc>'
"command! DelSwap exe '! rm '.shellescape(swapname(bufnr()), 1)

nnoremap q: <nop>
nnoremap Z <nop>
nnoremap <c-w>t <cmd>exe 'tabe % \| call winrestview('.string(winsaveview()).')'<cr>
nnoremap <c-w>m <c-w>s<c-w>_
vnoremap * ""y/<c-r>"<cr>
vnoremap # ""y?<c-r>"<cr>
cnoremap <c-a> <c-b>
nnoremap <c-a> ggVG
vnoremap <c-c> "+y
vnoremap <c-x> "+d
vnoremap <c-v> "+p
inoremap <c-v> <cmd>set paste<cr><c-r>+<cmd>set nopaste<cr>
if has('nvim')
	tnoremap <esc> <c-\><c-n>
	unmap Y
endif

nnoremap <a-u> :noh<cr>
nnoremap <a-s> /\s\+$<cr>

set spelllang=en_us,ru
"set spellfile=~/.vim/spell/filemanager.utf-8.add
set spellfile=~/.vim/spell/custom.utf-8.add
set encoding=utf-8
inoremap <c-s> <cmd>set invspell spell?<cr>
nnoremap <c-s> <cmd>set invspell spell?<cr>

:map  <MiddleMouse>   <Nop>
:map! <MiddleMouse>   <Nop>
:map  <2-MiddleMouse> <Nop>
:map! <2-MiddleMouse> <Nop>
:map  <3-MiddleMouse> <Nop>
:map! <3-MiddleMouse> <Nop>
:map  <4-MiddleMouse> <Nop>
:map! <4-MiddleMouse> <Nop>

aug vimrc_general_config
	au!
	au BufLeave * stopinsert
	if has('nvim')
		au TermOpen * setl nonumber
		au TermOpen * startinsert
	endif
aug END

aug vimrc_filetype
	au!
	au BufNewFile,BufRead *.letter,*.email setf readable
	au BufNewFile,BufRead {README,TODO}{,.*} runtime ftplugin/readable.vim
aug END

let g:tex_flavor = "latex"

let g:filemanager_sortrules = {'/figures$': 'obey:*/,\.tex$,*,.*/,.*', '/Downloads$': 'time'}
let g:filemanager_alwaysexternal = '\.pdf$'

colorscheme tuning

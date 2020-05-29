" nvim-qt does not respect Cursor highlighting and guicursor variable
" https://github.com/neovim/neovim/issues/8715
"
" additional bash-completion works with colors when I execute a script after bash has started
set number
set mouse=a
set autochdir
set history=50
set showcmd
set whichwrap+=[,] " wrap lines with arrow keys in insert mode
set hlsearch
set noincsearch
set virtualedit=block
set wildmenu
set complete+=kspell,i
set cursorline
set path+=**

set shortmess-=F
set fillchars=fold:-
set title
set guicursor=n-v-c-sm:block,i-ci-ve:ver25,r-cr-o:hor20

set wrap
set linebreak
set breakindent
set showbreak=\| 

set tabstop=2
set softtabstop=2
set shiftwidth=2
set nosmarttab
set autoindent

set foldmethod=marker
filetype on
syntax on
filetype plugin on
filetype indent on
set formatoptions=croqwjn1
" format comments; add comment leader on <cr> and o; format with gq; use 
" trailing space to indicate paragraph continuation; remove comment splitting 
" wisely; format numbered lists; don't split lines of single-letter-words
set nojoinspaces


command Q q
command W w
command D e %:p:h
command Xx ! xdg-open %:p &>/dev/null & disown
command -nargs=? M  wa <bar> ! make <args>
command -nargs=1 Oline exe "normal <esc>YPVr".<f-args>."Y"
command -nargs=1 Uline exe "normal <esc>YpVr".<f-args>."Y"
command Terminal tabnew <bar> terminal

set laststatus=2
set statusline=%F\ %m%r%h%w%=
set statusline+=%-6k\ line\ %l,\ col\ %v\ %4P\  
" virtual column number %v is better than the actual %c for unicode

nnoremap q: <nop>
vnoremap q gq
noremap ' `
noremap ` '
noremap <c-a> <esc>ggVG
tnoremap <esc> <c-\><c-n>
vnoremap <c-c> "+y
vnoremap <c-x> "+d
"nnoremap <c-v> "+p
inoremap <c-v> <c-r><c-o>+
vnoremap <c-v> "+p

nnoremap <a-u> :noh<cr>
inoremap <a-u> <c-o>:noh<cr>
nnoremap <a-s> / $<cr>
nnoremap <a-s> <c-o>/ $<cr>

set spelllang=en_us,ru
set spellfile=~/.vim/spell/custom.utf-8.add
set encoding=utf-8
inoremap <c-s> <c-o>:silent let &spell=!&spell <bar> echo "spell =" &spell <Enter>
nnoremap <c-s> :silent let &spell=!&spell <bar> echo "spell =" &spell <Enter>

let g:tex_flavor = "latex"

"let g:netrw_browsex_viewer = "mimeopen -n"
let g:netrw_browse_split = 3
let g:netrw_liststyle = 1
let g:netrw_sizestyle = "H"
let g:netrw_mousemaps = 0
let g:netrw_winsize = 70
aug vimrc_netrw
	au!
	au FileType netrw setl bufhidden=delete
aug END

"if argc() > 1 | silent tab all | endif

augroup local_insert
	au!
	au BufLeave * stopinsert
augroup END


aug vimrc_filetype
	au! 
	au BufNewFile,BufRead *.letter,*.email setf email
aug END

command TW doau BufRead an.email

colorscheme tuning
runtime russian.vim
runtime tabline.vim

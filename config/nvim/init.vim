" nvim-qt does not respect Cursor highlighting and guicursor variable
" https://github.com/neovim/neovim/issues/8715
" Put folders first in netrw
set number
set mouse=a
set autochdir
set history=50
set showcmd
set whichwrap+=[,] " wrap lines with arrow keys in insert mode
set hlsearch
set noincsearch
set ignorecase
set smartcase
set virtualedit=block
set wildmenu
set complete+=kspell,i
set cursorline
set path+=**

set shortmess-=F
set scrolloff=2
set fillchars=fold:-
set title
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

set foldmethod=marker
filetype on
syntax on
filetype plugin on
filetype indent on
set formatoptions=croqwjn1
" format comments; add comment leader on <cr> and o; format with gq; use 
" trailing space to indicate paragraph continuation; remove comment splitting 
" wisely; format numbered lists; don't split lines at single-letter-words
set nojoinspaces

command! -bang W w<bang>
command! -bang Q q<bang>
command! D e %:p:h
command! EE update <bar> e %
command! -nargs=? M  wa <bar> ! make <args>
command! -nargs=1 -complete=file Open update <bar> ! cd %:p:h; execbg xdg-open <q-args>
command! -nargs=1 Oline exe "normal <esc>YPVr=0R".<q-args>." <esc>Y"
command! -nargs=1 Uline exe "normal <esc>YpVr=0R".<q-args>." <esc>Y"
command! Paste set invpaste paste?
command! Terminal tabnew <bar> terminal

nnoremap q: <nop>
vnoremap q gq
noremap ' `
noremap ` '
nnoremap <c-a> ggVG
tnoremap <esc> <c-\><c-n>
unmap Y
vnoremap <c-c> "+y
vnoremap <c-x> "+d
vnoremap <c-v> "+p
"nnoremap <c-v> "+p
inoremap <c-v> <space><bs><esc>"+pa

nnoremap <a-u> :noh<cr>
inoremap <a-u> <c-o>:noh<cr>
nnoremap <a-s> / $<cr>
inoremap <a-s> <c-o>/ $<cr>

set laststatus=2
let &statusline=' %F %m%r%h%w%=%-6k %-16(%l,%c%V / %LL%) %P '

set spelllang=en_us,ru
set spellfile=~/.vim/spell/custom.utf-8.add
set encoding=utf-8
inoremap <c-s> <c-o>:set invspell spell?<cr>
nnoremap <c-s> :set invspell spell?<cr>

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

aug local_insert
	au!
	au BufLeave * stopinsert
aug END


let g:tex_flavor = "latex"

aug vimrc_filetype
	au!
	au BufNewFile,BufRead *.letter,*.email setf readable

	" match any UPPERCASE[.extension] filename or only specific ones.
	"au BufNewFile,BufRead README*,TODO* runtime ftplugin/readable.vim
	au BufNewFile,BufRead [A-Z_-]\\\{1,\}{,.*} runtime ftplugin/readable.vim
aug END

command! Readable doau BufRead an.email
nnoremap <a-r> :silent Readable<cr>

colorscheme tuning
runtime russian.vim
runtime tabline.vim

"" GNVIM Configuration
""""""""""""""""""""""
if exists("gnvim")
	call gnvim#enable_ext_tabline(0)
	colorscheme gui_github
endif

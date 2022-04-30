setl spell
syn spell toplevel
setl ts=2 sts=0 sw=0 noet nosta
setl tw=72 wm=1

" === Compiling commands: simple files, bibtex involving, Overleaf... ===
command! -buffer -complete=file -nargs=1 -bar Latex ! pdflatex
	\ -file-line-error -interaction=nonstopmode <args>
        \ <bar> sed -n -e '/\.\/.*\.tex:/{h' -e ':i' -e 'n;H;/^ *$/{g;p;d};bi}'
command! -buffer -complete=file -nargs=1 -bar Latexwarnings ! pdflatex
	\ -file-line-error -interaction=nonstopmode <args>
        \ <bar> sed -n -e '/\.\/.*\.tex:\|[Ww]arning/{h' -e ':i' -e 'n;H;/^ *$/{g;p;d};bi}'
command! -buffer -complete=file -nargs=1 -bar Latexfull ! pdflatex -file-line-error -interaction=nonstopmode <args>
command! -buffer -complete=file -nargs=1 -bar Bibtex ! bibtex <args>
command! -buffer -bar R update <bar> Latex %
command! -buffer -bar Rw update <bar> Latexwarnings %
command! -buffer -bar Rfull update <bar> Latexfull %
command! -buffer -bar C ! texclean %
command! -buffer -bar Ca ! texclean *.tex
command! -buffer RRC R | R | C
command! -buffer RB C | R | Bibtex %:r | R | R | C
command! -buffer B RB
command! -buffer P silent ! setsid -f xdg-open %:p:r.pdf </dev/null >/dev/null 2>&1

command! -buffer -bar OR wa <bar> Latex main
command! -buffer -bar ORw wa <bar> Latexwarnings main
command! -buffer ORB wa | Ca | OR | Bibtex main | OR | OR
command! -buffer OB ORB
command! -buffer Overleaf wa | Ca | OR | Bibtex main | OR | OR | Ca
command! -buffer Opdf silent ! setsid -f xdg-open main.pdf </dev/null >/dev/null 2>&1

" === General mappings ===
vnoremap <buffer> <c-t> :'<,'>! column -t -s \& -o \& <cr>
vnoremap <buffer> a$ F$of$
vnoremap <buffer> i$ F$lof$h

" === Modifying commands: default files, environments, wrappers ===
command! -buffer Default r ~/work/tex/default.tex | normal! kdd
command! -buffer Twocolumn r ~/work/tex/twocolumn.tex | normal! kdd
command! -buffer Presentation r ~/work/tex/presentation.tex | normal! kdd
command! -buffer Figure call s:Figure()
command! -buffer SubFigure call s:SubFigure()
command! -buffer Table call s:Table()
command! -buffer Frame call s:Frame()
command! -buffer Initials s/\. /\.\~/g
command! -buffer -nargs=* Environment call s:env(<f-args>)
command! -buffer RenameEnvironment call s:renameenv()

" === Mappings for modifying commands ===
nnoremap <buffer> glf <cmd>Figure<cr>
nnoremap <buffer> gls <cmd>SubFigure<cr>
nnoremap <buffer> glt <cmd>Table<cr>
nnoremap <buffer> glF <cmd>Frame<cr>
nnoremap <buffer> glI <cmd>Initials<cr>
nnoremap <buffer> gle <cmd>Environment equation*<cr>
nnoremap <buffer> gln <cmd>Environment enumerate<cr>
nnoremap <buffer> gla <cmd>Environment enumerate label=alph*)<cr>
nnoremap <buffer> gli <cmd>Environment itemize<cr>
nnoremap <buffer> glE <cmd>Environment<cr>
nnoremap <buffer> glR <cmd>call <sid>renameenv()<cr>
nnoremap <buffer> glr <cmd>call <sid>renameenv()<cr>

" === Automatic formatting ===
setl fo-=t
setl fo-=a
inoremap <buffer> <c-y> <c-o>:setl fo+=ta<cr>i<bs>
aug tex_formatting
	au!
	au InsertLeave <buffer> exe 'setl fo-=t' | exe 'setl fo-=a'
aug END


" === General functions and env-specific ones ===
fun! s:env(...)
	let l:env = get(a:000, 0, '')
	if len(l:env) == 0
		let l:env = input('Environment: ')
	endif
	if len(l:env) == 0
		return
	endif
	if len(a:000) > 1
		let l:opt = '['.join(a:000[1:], ', ').']'
	else
		let l:opt = ''
	endif
	call append('.', ['\begin{'.l:env.'}'.l:opt, '\end{'.l:env.'}'])
	normal! jV1j=
endfun

let s:envpos = []
fun! s:renameenv()
	let l:line1 = line('.')
	let l:col1 = col('.')
	call cursor(l:line1, 1)
	if search('\\begin{\|\\end{', 'c', l:line1) == 0
		echo 'No environment found in current line.'
		return
	endif
	call cursor(l:line1, l:col1)
	normal %
	let l:line2 = line('.')
	let l:col2 = col('.')
	normal %
	let l:line1 = line('.')
	let l:col1 = col('.')

	let l:reg = getreg('"')
	normal! f{viB""y
	let l:oldenv = getreg('"')
	call setreg('"', l:reg)

	call inputsave()
	let l:newenv = input('Enter new env. name: ', l:oldenv)
	call inputrestore()

	if l:newenv == ''
		echo 'Empty name supplied. Aborting'
		return
	endif

	call cursor(l:line1, l:col1)
	exe 'normal! f{viBs'.l:newenv
	call cursor(l:line2, l:col2)
	exe 'normal! f{viBs'.l:newenv
	call cursor(l:line1, l:col1)
endfun

fun! s:Frame()
	call append('.',
		\ [ '\frame{\frametitle{}'
		\ , '}'])
	normal! jV1j=V1jzfzoj<<kf}
endfun

fun! s:Figure()
	call append('.',
		\ [ '\begin{figure}'
		\ , '	\centering'
		\ , '	\includegraphics[width=\linewidth]{figures/}'
		\ , '	\caption{}'
		\ , '	\label{fig:}'
		\ , '\end{figure}'])
	normal! jV5j=V5jzfzof%l
endfun

fun! s:SubFigure()
	call append('.',
		\ [ '	\null\hfill'
		\ , '	\begin{subfigure}{.4\linewidth}'
		\ , '		\includegraphics[width=\linewidth]{figures/}'
		\ , '		\caption{}'
		\ , '		\label{fig:}'
		\ , '	\end{subfigure}'
		\ , '	\hfill\null'])
	normal! jV6j=
endfun

fun! s:Table()
	call append('.',
		\ [ '\begin{table}'
		\ , '	\centering'
		\ , '	\caption{}'
		\ , '	\label{tab:}'
		\ , '	\begin{tabular}{}'
		\ , '	\end{tabular}'
		\ , '\end{table}'])
	normal! jV6j=V6jzfzof%l
endfun

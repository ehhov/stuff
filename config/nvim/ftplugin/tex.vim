setl spell
syn spell toplevel
let b:did_indent = 1
setl ts=2 sts=0 sw=0 noet nosta
setl tw=72 wm=1

command! -buffer R update <bar> ! pdflatex -file-line-error -interaction=nonstopmode % <args> <bar> grep --color ':.*:'
command! -buffer C ! texclean %
command! -buffer RRC exe 'R' <bar> exe 'R' <bar> exe 'C'
command! -buffer RB exe 'C' <bar> exe 'R' <bar> exe '! bibtex %:r' <bar> exe 'R' <bar> exe 'R' <bar> exe 'C'
command! -buffer P silent ! execbg xdg-open %:p:r.pdf

vnoremap <buffer> <c-t> :'<,'>! column -t -s \& -o \& <cr>
inoremap <buffer> <c-e> <esc>YPVr%Y
vnoremap <buffer> a$ F$of$
vnoremap <buffer> i$ F$lof$h
inoremap <buffer> <c-y> <c-o>:set fo+=ta <cr>i<bs>

setl fo-=t
setl fo-=a

aug tex_formatting
	au!
	au InsertLeave <buffer>
		\ exe 'set fo-=t' |
		\ exe 'set fo-=a' |
aug END

" Functions for copy-paste shortcuts
fun! Figure()
	call append('.',
		\ [ '\begin{figure}% {{{'
		\ , '	\centering'
		\ , '	\includegraphics[width=\linewidth]{figures/}'
		\ , '	\caption{}'
		\ , '	\label{fig:}'
		\ , '\end{figure}% }}}'])
endfun

fun! SubFigure()
	call append('.',
		\ [ '	\hfill'
		\ , '	\begin{subfigure}{.4\linewidth}'
		\ , '		\includegraphics[width=\linewidth]{figures/}'
		\ , '		\caption{}'
		\ , '		\label{fig:}'
		\ , '	\end{subfigure}'
		\ , '	\hfill'])
endfun

fun! ListAlph()
	call append('.',
		\ [ '\begin{enumerate}[label=\alph*)]'
		\ , '\end{enumerate}'])
endfun

fun! Table()
	call append('.',
		\ [ '\begin{table}% {{{'
		\ , '	\centering'
		\ , '	\caption{}'
		\ , '	\label{tab:}'
		\ , '	\begin{tabular}{}'
		\ , '	\end{tabular}'
		\ , '\end{table}% }}}'])
endfun

fun! Initials()
	s/\. /\.\~/g
endfun

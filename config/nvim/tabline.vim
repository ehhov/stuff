set showtabline=1
nnoremap <a-1> :tabnext 1 <cr>
nnoremap <a-2> :tabnext 2 <cr>
nnoremap <a-3> :tabnext 3 <cr>
nnoremap <a-4> :tabnext 4 <cr>
nnoremap <a-5> :tabnext 5 <cr>
nnoremap <a-6> :tabnext 6 <cr>
nnoremap <a-7> :tabnext 7 <cr>
nnoremap <a-8> :tabnext 8 <cr>
nnoremap <a-9> :tabnext 9 <cr>
nnoremap <a-0> :tabnext 10<cr>

nnoremap <a-t> :tabnew<cr>
nnoremap <a-p> :tabp<cr>
nnoremap <a-n> :tabn<cr>
nnoremap <a-s-p> :tabm-<cr>
nnoremap <a-s-n> :tabm+<cr>

let g:lasttab = 1
aug lasttab_tabline
	au!
	au TabLeave * let g:lasttab = tabpagenr()
aug END
nnoremap <a-l> :exe "tabnext ".g:lasttab<cr>
inoremap <a-l> <c-o>:exe "tabnext ".g:lasttab<cr>

fun! TheTabLine()
	let attr = []
	let text = []

  for tab in range(tabpagenr('$'))
		let a = '' | let t = ''

    let tab = tab + 1
    let buflist = tabpagebuflist(tab)
		let bufnr = buflist[tabpagewinnr(tab) - 1]
    let bufname = bufname(bufnr)
		for i in buflist
			let bufmodified = getbufvar(i, "&mod")
			if bufmodified | break | endif
		endfor
		let bufft = getbufvar(bufnr, "&filetype")

    let a .= '%' . tab . 'T'
    let a .= (tab == tabpagenr() ? '%#TabLineSel#' : '%#TabLine#')
    let t .= ' ' . tab . ' '
    let t .= (bufname != '' ? fnamemodify(bufname, ':t') : '[No Name]')
		let t .= (bufft == 'netrw' ? '/' : '')
		let i  = (len(buflist)>1? len(buflist):'').(bufft=='help'? 'h':'').(bufmodified? '+':'')
		let t .= (len(i)>0? ' ['.i.']' : '')
    let t .= (tab == tabpagenr() || tab == tabpagenr()-1 || tab == tabpagenr('$') ? ' ' : ' |')

		let attr += [a]
		let text += [t]
  endfor

	if len(join(text, '')) > &columns && tabpagenr('$') > 1
		let isfull = [] | let l = 0
		for i in range(tabpagenr('$'))
			let isfull += [0]
			if i == tabpagenr()-1 | continue | endif
			let text[i] = (i == tabpagenr()-2 || i == tabpagenr('$')-1 ? text[i][1:-2] : text[i][1:-3].'|')
		endfor
		let isfull[tabpagenr()-1] = 1
		while 1
			let l = &columns
			for i in range(tabpagenr('$'))
				let l -= len(text[i])*isfull[i]
			endfor
			exe 'let sum = ' . tabpagenr('$') . '-'. join(isfull, '-')
			if sum>0 | let l = l / sum | else | let l = &columns | endif
			for i in range(tabpagenr('$'))
				if i == tabpagenr()-1 | continue | endif
				let isfull[i] = (l >= len(text[i]))
			endfor

			exe 'let q = ' . join(isfull, '+') .'+'. (sum-tabpagenr('$'))
			if q <= 0 | break | endif
		endwhile
		let l = l/2
		let c = &columns
		for i in range(tabpagenr('$'))
			let c -= len(text[i])*isfull[i]
		endfor
		let c -= 2*l*sum
		for i in range(tabpagenr('$'))
			if !isfull[i]
				let t = text[i]
				let text[i] =  t[:l+(c>0)-1] . '*'
				let c -= 1
				let text[i].= t[-l-(c>0)+1:]
				let c -= 1
			endif
			" … · * .
		endfor
	endif

	let line = ''
	for i in range(tabpagenr('$'))
		let line .= attr[i] . text[i]
	endfor
  return line . '%#TabLineFill#%999T%='
endfun

set tabline=%!TheTabLine()

finish
" The following is an attempt to make a different type of shortening


		let l = (&columns - len(text[tabpagenr()-1]))/2
		let tab = tabpagenr() - 1 | if tab == 0 | let tab = 1 | endif
		if len(join(text[:tab-1], '')) < l
			let ll = &columns - len(join(text[:tabpagenr()-1],''))
			for i in range(tabpagenr(), tabpagenr('$')-1)
				if ll >= len(text[i])
					let ll -= len(text[i])
				else
					let text[i] = text[i][:ll-1]
					let ll = 0
				endif
			endfor
			if ll == 0 | let text[-1] = text[-1][:-2].'B' | endif
		elseif len(join(text[tabpagenr():], '')) < l
			let ll = &columns - len(join(text[tabpagenr()-2:],''))
			for i in range(tabpagenr()-2,0, -1)
				if ll >= len(text[i])
					let ll -= len(text[i])
				else
					let text[i] = text[i][-ll-1:]
					let ll = 0
				endif
			endfor
			if ll == 0 | let text[0] = 'E'.text[0][1:] | endif
		else
			let ll = l
			for i in range(tabpagenr()-1,0, -1)
				if ll >= len(text[i])
					let ll -= len(text[i])
				else
					let text[i] = text[i][-ll-1:]
					let ll = 0
				endif
			endfor
			if ll == 0 | let text[0] = 'M'.text[0][1:] | endif
			let ll = l
			for i in range(tabpagenr(), tabpagenr('$')-1)
				if ll >= len(text[i])
					let ll -= len(text[i])
				else
					let text[i] = text[i][:ll-1]
					let ll = 0
				endif
			endfor
			if ll == 0 | let text[-1] = text[-1][:-2].'M' | endif
		endif

set showtabline=1
nnoremap  <a-1>  <cmd>tabnext 1<cr>
nnoremap  <a-2>  <cmd>tabnext 2<cr>
nnoremap  <a-3>  <cmd>tabnext 3<cr>
nnoremap  <a-4>  <cmd>tabnext 4<cr>
nnoremap  <a-5>  <cmd>tabnext 5<cr>
nnoremap  <a-6>  <cmd>tabnext 6<cr>
nnoremap  <a-7>  <cmd>tabnext 7<cr>
nnoremap  <a-8>  <cmd>tabnext 8<cr>
nnoremap  <a-9>  <cmd>tablast<cr>

nnoremap  <a-t>    <cmd>tabnew<cr>
nnoremap  <a-w>    <cmd>tabclose<cr>
nnoremap  <a-p>    <cmd>tabprevious<cr>
nnoremap  <a-n>    <cmd>tabnext<cr>
nnoremap  <a-s-p>  <cmd>tabmove -1<cr>
nnoremap  <a-s-n>  <cmd>tabmove +1<cr>
nnoremap  <a-l>    g<tab>
inoremap  <a-l>    <c-o>g<tab>

fun! TheTabLine()
	let attr = []
	let text = []
	let fintab = tabpagenr('$')
	let curtab = tabpagenr()

	for tab in range(1, fintab)
		let a = '' | let t = ''

		let buflist = tabpagebuflist(tab)
		let bufnr = buflist[tabpagewinnr(tab)-1]
		for i in buflist
			let bufmodified = getbufvar(i, "&mod")
			if bufmodified | break | endif
		endfor
		let bufname = bufname(bufnr)
		if bufname == ''
			let bufname = '[No Name]'
		elseif fnamemodify(bufname, ':t') == ''
			let bufname = fnamemodify(bufname, ':h:t').'/'
		else
			let bufname = fnamemodify(bufname, ':t')
		endif

		let a .= '%' . tab . 'T'
		let a .= (tab == curtab ? '%#TabLineSel#' : '%#TabLine#')
		let t .= ' ' . tab . ' '
		let t .= bufname
		let i  = (len(buflist)>1? len(buflist):'').(bufmodified? '+':'')
		let t .= (len(i)>0? ' ['.i.']' : '')
		let t .= (tab == curtab || tab == curtab-1 || tab == fintab ? ' ' : ' |')

		let attr += [a]
		let text += [t]
	endfor

	if len(join(text, '')) > &columns && fintab > 1
		let isfull = [] | let l = 0
		for i in range(fintab)
			let isfull += [0]
			if i == curtab-1 | continue | endif
			let text[i] = (i == curtab-2 || i == fintab-1 ? text[i][1:-2] : text[i][1:-3].'|')
		endfor
		let isfull[curtab-1] = 1
		while 1
			let l = &columns
			for i in range(fintab)
				let l -= len(text[i])*isfull[i]
			endfor
			exe 'let sum = ' . fintab . '-'. join(isfull, '-')
			if sum>0 | let l = l / sum | else | let l = &columns | endif
			for i in range(fintab)
				if i == curtab-1 | continue | endif
				let isfull[i] = (l >= len(text[i]))
			endfor

			exe 'let q = ' . join(isfull, '+') .'+'. (sum-fintab)
			if q <= 0 | break | endif
		endwhile
		let l = l/2
		let c = &columns
		for i in range(fintab)
			let c -= len(text[i])*isfull[i]
		endfor
		let c -= 2*l*sum
		for i in range(fintab)
			if !isfull[i]
				let t = text[i]
				let text[i] = t[:l+(c>0)-1] . '*'
				let c -= 1
				let text[i].= t[-l-(c>0)+1:]
				let c -= 1
			endif
			" … · * .
		endfor
	endif

	let line = ''
	for i in range(fintab)
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

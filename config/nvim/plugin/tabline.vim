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
inoremap  <a-l>    <esc>g<tab>

nnoremap  <c-w>,  <cmd>call <sid>settabname()<cr>


fun! s:settabname()  " {{{
	call inputsave()
	let l:existed = exists('t:tabname') && !empty(t:tabname)
	if l:existed
		echo 'Current tab name:  "'.t:tabname.'"'
		let l:tabname = input('Enter new tab name: ', t:tabname)
	else
		let l:tabname = input('Enter tab name: ')
	endif
	call inputrestore()
	if empty(l:tabname)
		redraw
		if l:existed
			echo 'Empty name supplied. Tab name reset'
			unlet t:tabname
		else
			echo 'Empty name supplied. Aborted'
			return
		endif
	else
		let t:tabname = l:tabname
	endif
	redraw!
endfun  " }}}


fun! s:getbufname(bufnr)  " {{{
	if empty(bufname(a:bufnr))
		return '[No Name]'
	elseif empty(fnamemodify(bufname(a:bufnr), ':t'))
		return fnamemodify(bufname(a:bufnr), ':h:t').'/'
	else
		return fnamemodify(bufname(a:bufnr), ':t')
	endif
endfun  " }}}


fun! s:tabtext(tabnr)  "{{{
	let l:buflist = tabpagebuflist(a:tabnr)
	let l:bufnr = buflist[tabpagewinnr(a:tabnr)-1]
	let l:bufname = gettabvar(a:tabnr, 'tabname', s:getbufname(l:bufnr))
	let l:info = (len(l:buflist) > 1 ? len(l:buflist) : '')
	            \.(empty(filter(l:buflist, 'getbufvar(v:val, "&mod")')) ? '' : '+')
	return ' '.a:tabnr.' '.l:bufname.(empty(l:info) ? '' : ' ['.l:info.']').' '
endfun  "}}}


fun! s:getmaxlen(tabs, fulllen)  "{{{
	let l:len = a:fulllen / (len(a:tabs) + 1)
	let l:smallestnotfull = 0
	while l:len > l:smallestnotfull
		let l:newlen = a:fulllen
		let l:notfull = len(a:tabs)
		let l:smallestnotfull = a:fulllen
		for l:tab in a:tabs
			if l:len >= len(l:tab)
				let l:newlen -= len(l:tab)
				let l:notfull -= 1
			elseif l:smallestnotfull > len(l:tab)
				let l:smallestnotfull = len(l:tab)
			endif
		endfor
		let l:len = l:notfull > 0 ? l:newlen / l:notfull : a:fulllen
	endwhile
	return l:len
endfun  "}}}


fun! s:shrink(tabs, fulllen)  "{{{
	let l:maxlen = s:getmaxlen(a:tabs, a:fulllen)
	let l:half = l:maxlen / 2
	let l:half = l:half > 1 ? l:half : 2
	let l:cols = a:fulllen
	for l:tab in a:tabs
		let l:cols -= l:maxlen >= len(l:tab) ? len(l:tab) : 2*l:half
	endfor
	for l:i in range(len(a:tabs))
		if l:maxlen < len(a:tabs[l:i])
			let a:tabs[l:i] = a:tabs[l:i][:l:half-1+(l:cols>0)].'*'
			                 \.a:tabs[l:i][-l:half+1-(l:cols>1):]
			let l:cols -= 2
		endif
	endfor
endfun  "}}}


fun! TabLineFunction()  " {{{
	let l:tabs = map(range(1, tabpagenr('$')), 's:tabtext(v:val)')

	if len(join(l:tabs, ' ')) > &columns && len(l:tabs) > 1
		call map(l:tabs, 'v:key == '.(tabpagenr()-1).' ? v:val : v:val[1:-2]')
		let l:curtab = remove(l:tabs, tabpagenr()-1)
		if len(join(l:tabs, ' ')) + 1 + len(l:curtab) > &columns && len(l:tabs) > 1
			call s:shrink(l:tabs, &columns - len(l:tabs) - len(l:curtab))
		else
			let l:cols = &columns - len(join(l:tabs, ' ')) - 1 - len(l:curtab)
			call map(l:tabs, '(l:cols - 2*v:key > 0 ? " " : "").v:val.(l:cols - 2*v:key > 1 ? " " : "")')
		endif
		call insert(l:tabs, l:curtab, tabpagenr()-1)
	endif

	return join(map(l:tabs, '"%".(v:key+1)."T%#TabLine".(v:key == '.(tabpagenr()-1).' ? "Sel" : "")."#".v:val'),
	            \'%#TabLineFill# ') . '%#TabLineFill#%999T%='
endfun  " }}}


set tabline=%!TabLineFunction()


finish
" The following is an attempt to make a different type of shortening {{{


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

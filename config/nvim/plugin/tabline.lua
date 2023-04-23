-- Vim and Neovim Tab Line. Rewritten in Lua


local main, ondemand, settabname
local getbufname, gettabtext
local unspace, getmaxlen, shrink
local nonprintable, tabslen


function main()
	vim.api.nvim_set_option('tabline', '%!v:lua.tabline()')
	vim.api.nvim_set_option('showtabline', 1)

	vim.keymap.set('n', '<a-1>',    ondemand(vim.cmd.tabnext, 1))
	vim.keymap.set('n', '<a-2>',    ondemand(vim.cmd.tabnext, 2))
	vim.keymap.set('n', '<a-3>',    ondemand(vim.cmd.tabnext, 3))
	vim.keymap.set('n', '<a-4>',    ondemand(vim.cmd.tabnext, 4))
	vim.keymap.set('n', '<a-5>',    ondemand(vim.cmd.tabnext, 5))
	vim.keymap.set('n', '<a-6>',    ondemand(vim.cmd.tabnext, 6))
	vim.keymap.set('n', '<a-7>',    ondemand(vim.cmd.tabnext, 7))
	vim.keymap.set('n', '<a-8>',    ondemand(vim.cmd.tabnext, 8))
	vim.keymap.set('n', '<a-9>',    ondemand(vim.cmd.tablast))

	vim.keymap.set('n', '<a-t>',    ondemand(vim.cmd.tabnew))
	vim.keymap.set('n', '<a-w>',    ondemand(vim.cmd.tabclose))
	vim.keymap.set('n', '<a-p>',    ondemand(vim.cmd.tabprevious))
	vim.keymap.set('n', '<a-n>',    ondemand(vim.cmd.tabnext))
	vim.keymap.set('n', '<a-s-p>',  ondemand(vim.cmd.tabmove, '-'))
	vim.keymap.set('n', '<a-s-n>',  ondemand(vim.cmd.tabmove, '+'))
	vim.keymap.set('n', '<a-l>',    'g<tab>')
	vim.keymap.set('i', '<a-l>',    '<esc>g<tab>')

	vim.keymap.set('n', '<c-w>,',   settabname)
end


function ondemand(fn, args)
	return function()
		local ok, err = pcall(fn, args)
		if not ok and err then
			err, _ = err:gsub('^Vim%(?%a*%)?:', '')
			vim.api.nvim_err_writeln(err)
		end
	end
end


function settabname()
	local tabname
	local existed = vim.t.tabname ~= nil and vim.t.tabname ~= ''
	vim.fn.inputsave()
	if existed then
		vim.api.nvim_echo({{'Current tab name:  '..vim.t.tabname}},
		                  false, {})
		vim.ui.input({prompt='Enter new tab name: ',
		              default=vim.t.tabname},
		             function(str) tabname = str end)
	else
		vim.ui.input({prompt='Enter new tab name: '},
		             function(str) tabname = str end)
	end
	vim.fn.inputrestore()
	vim.cmd.redraw()
	if tabname ~= '' then
		vim.t.tabname = tabname
	elseif existed then
		vim.api.nvim_echo({{'Empty name supplied. Tab name reset'}},
		                  false, {})
		vim.t.tabname = nil
	else
		vim.api.nvim_echo({{'Empty name supplied. Aborted'}},
		                  false, {})
	end
	vim.cmd.redrawtabline()
end


function getbufname(bufnr)
	-- nvim_buf_get_name() returns a modified "path" buf name, not plain
	local name = vim.fn.bufname(bufnr)
	if name == '' then return '[No Name]' end
	local basename = vim.fs.basename(name)
	if basename ~= '' then return basename end
	return vim.fs.basename(vim.fs.dirname(name))..'/'
end


function gettabtext(nr, tabid)
	local buflist = {}
	for _, winid in pairs(vim.api.nvim_tabpage_list_wins(tabid)) do
		buflist[#buflist+1] = vim.api.nvim_win_get_buf(winid)
	end
	local ok, bufname = pcall(vim.api.nvim_tabpage_get_var, tabid, 'tabname')
	if not ok or not bufname then
		local winid = vim.api.nvim_tabpage_get_win(tabid)
		bufname = getbufname(vim.api.nvim_win_get_buf(winid))
	end
	local info = (#buflist > 1 and #buflist or '')
	for _, nr in pairs(buflist) do
		if vim.api.nvim_buf_get_option(nr, 'modified') then
			info = info..'+'
			break
		end
	end
	if info ~= '' then info = ' ['..info..']' end
	return ' '..nr..' '..bufname..info..' '
end


function unspace(tabs, tabsel, columns)
	local spare = columns - tabslen(tabs) + 2*#tabs - 2
	for k, v in pairs(tabs) do if k ~= tabsel then
		tabs[k] = v:sub(spare > 0 and 1 or 2, spare > 1 and -1 or -2)
		spare = spare - 2
	end end
end


function getmaxlen(tabs, tabsel, avail)
	local len = avail / (#tabs - 1)
	local smallest = 0
	while len > smallest do
		local newlen = avail
		local nlarger = #tabs - 1
		smallest = avail
		for k, v in pairs(tabs) do
			if len > #v and k ~= tabsel then
				newlen = newlen - #v
				nlarger = nlarger - 1
			elseif smallest > #v and k ~= tabsel then
				smallest = #v
			end
		end
		len = nlarger > 0 and newlen / nlarger or avail
	end
	return len
end


function shrink(tabs, tabsel, columns)
	local avail = columns - #tabs + 1 - #tabs[tabsel]
	local maxlen = getmaxlen(tabs, tabsel, avail)
	local half = math.floor(maxlen / 2)
	half = half > 1 and half or 2
	local spare = avail + #tabs[tabsel]
	for k, v in pairs(tabs) do
		spare = spare - ((maxlen >= #v or k == tabsel)
		                 and #v or 2*half)
	end
	for k, v in pairs(tabs) do if maxlen < #v and k ~= tabsel then
		tabs[k] = v:sub(1, half + (spare > 0 and 1 or 0))..
		          '*'.. v:sub(-half+1 - (spare > 1 and 1 or 0))
		spare = spare - 2
	end end
end


function nonprintable(nr, selnr)
	return '%'..nr..'T%#TabLine'..(nr == selnr and 'Sel' or '')..'#'
end


function tabslen(tabs)
	local sum = 0
	for _, tab in pairs(tabs) do
		sum = sum + #tab
	end
	return sum + #tabs - 1
end


function tabline()
	local tabs = {}
	local tabsel = vim.api.nvim_get_current_tabpage()
	tabsel = vim.api.nvim_tabpage_get_number(tabsel)

	for _, tabid in pairs(vim.api.nvim_list_tabpages()) do
		local nr = vim.api.nvim_tabpage_get_number(tabid)
		tabs[nr] = gettabtext(nr, tabid)
	end

	local columns = vim.api.nvim_get_option('columns')
	if tabslen(tabs) > columns and #tabs > 1 then
		unspace(tabs, tabsel, columns)
		if tabslen(tabs) > columns then
			shrink(tabs, tabsel, columns)
		end
	end

	for k, v in pairs(tabs) do
		tabs[k] = nonprintable(k, tabsel)..v
	end

	return table.concat(tabs, '%#TabLineFill# ')..'%#TabLineFill#%999T%='
end


main()

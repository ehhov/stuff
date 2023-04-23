-- Helper functions

local ecall

function ecall(fn, ...)
	local ok, err = pcall(fn, ...)
	if not ok then
		err, _ = err:gsub('^Vim%(?%a*%)?:', '')
		vim.notify(err, vim.log.levels.ERROR)
	end
end


-- Options
vim.o.mouse = 'a'
vim.o.number = true
vim.o.cursorline = true
vim.o.history = 50
vim.o.showcmd = true
vim.opt.whichwrap = {['[']=true, [']']=true}
vim.o.scrolloff = 2
vim.opt.fillchars = {fold='-'}
vim.o.title = true

vim.o.hlsearch = true
vim.o.incsearch = false
vim.o.ignorecase = true
vim.o.smartcase = true
vim.opt.virtualedit = {'block'}
vim.o.showmatch = false

vim.o.autochdir = true
vim.o.autoread = true
vim.o.hidden = false
vim.o.wildmenu = true
vim.opt.complete:append({'kspell', 'i'})
vim.opt.path:append('**')
vim.opt.shortmess:remove('F')

vim.o.wrap = true
vim.o.linebreak = true
vim.o.breakindent = true
vim.o.showbreak = '|'

vim.o.tabstop = 8
vim.o.softtabstop = 0
vim.o.shiftwidth = 0
vim.o.smarttab = true
vim.o.autoindent = true
vim.o.copyindent = true
vim.o.preserveindent = true

vim.o.foldmethod = 'marker'
vim.cmd.filetype('on')
vim.cmd.syntax('on')
vim.cmd.filetype('plugin on')
vim.cmd.filetype('indent on')
vim.o.joinspaces = false
vim.opt.formatoptions = {['c']=true, ['r']=true, ['o']=true,
                         ['q']=true,['j']=true, ['1']=true}

vim.o.laststatus = 2
vim.o.statusline = '%#StatusLineNC# %F %m%r%h%w %=%* %-5k%-16(%l,%c%V / %LL%) %P '
-- vim.o.statusline = '%='
-- vim.opt.fillchars:append({stl='─', stlnc='─'})


-- Mappings
vim.keymap.set('n', 'q:', function() end)
vim.keymap.set('n', 'Z',  function() end)
vim.keymap.set('n', '<c-w>t', function()
	local winview = vim.fn.winsaveview()
	ecall(vim.cmd.tabedit, vim.fn.expand('%'))
	vim.fn.winrestview(winview)
end)
vim.keymap.set('n', '<c-w>m', '<c-w>s<c-w>_')
vim.keymap.set('v', '*', '""y/<c-r>"<cr>')
vim.keymap.set('v', '#', '""y?<c-r>"<cr>')
vim.keymap.set('c', '<c-a>', '<c-b>')
vim.keymap.set('n', '<c-a>', 'ggVG')
vim.keymap.set('v', '<c-c>', '"+y')
vim.keymap.set('v', '<c-x>', '"+d')
vim.keymap.set('v', '<c-v>', '"+p')
vim.keymap.set('i', '<c-v>', function()
	vim.o.paste = true
	local lines = vim.split(vim.fn.getreg('+'), '\n', {plain=true})
	ecall(vim.paste, lines, -1)
	vim.o.paste = false
end)
vim.keymap.set('n', '<a-u>', vim.cmd.nohlsearch)
vim.keymap.set('n', '<a-s>', '/\\s\\+$<cr>')
vim.keymap.set({'n', 'i'}, '<c-s>', function()
	vim.o.spell = not vim.o.spell
	print((vim.o.spell and '  ' or 'no') .. 'spell')
end)
vim.keymap.set('t', '<esc>', '<c-\\><c-n>')

if vim.fn.hasmapto('Y') then
	vim.keymap.del('n', 'Y')
end

for _, v in pairs({'<MiddleMouse>', '<2-MiddleMouse>',
                   '<3-MiddleMouse>', '<4-MiddleMouse>'}) do
	vim.keymap.set({'n', 'v', 'o', 'i', 'c', 't', 'l'}, v, function() end)
end


-- Commands
vim.api.nvim_create_user_command('EE', function()
	ecall(vim.cmd.update)
	ecall(vim.cmd.edit)
end, {})

vim.api.nvim_create_user_command('Syn', function()
	vim.o.filetype = vim.o.filetype
end, {})

vim.api.nvim_create_user_command('Terminal', function()
	vim.cmd.tabnew()
	vim.cmd.terminal()
end, {})

vim.api.nvim_create_user_command('Notepad', function()
	vim.api.nvim_cmd({
		cmd='new',
		mods={split='belowright'}
	}, {})
	vim.cmd.resize(5)
	vim.bo.buftype = 'nofile'
	vim.wo.winfixheight = true
end, {})

vim.api.nvim_create_user_command('Justify', function()
	local line = vim.fn.getline(vim.fn.line('.'))
	local col = vim.fn.col('.')
	ecall(vim.fn.setline, vim.fn.line('.'), line:sub(1, col) ..
	      string.rep(' ', vim.o.textwidth - #line) .. line:sub(col+1))
end, {})

vim.api.nvim_create_user_command('M', function(table)
	ecall(vim.cmd.update)
	ecall(vim.cmd, '!make ' .. table.args)
end, {complete='file', nargs='?'})

vim.api.nvim_create_user_command('Mw', function(table)
	ecall(vim.cmd.wall)
	ecall(vim.cmd, '!make ' .. table.args)
end, {complete='file', nargs='?'})

vim.api.nvim_create_user_command('Count', function(table)
	ecall(vim.cmd, 'keeppatterns %s/' .. table.args .. '//gn')
	vim.cmd.nohlsearch()
end, {nargs='?'})

vim.api.nvim_create_user_command('HLSearch', function(table)
	vim.o.hlsearch = true
	vim.fn.setreg('/', table.args)
end, {nargs=1})

vim.api.nvim_create_user_command('Open', function(table)
	ecall(vim.cmd.update)
	ecall(os.execute, 'setsid -f xdg-open ' .. table.args ..
	      ' </dev/null >/dev/null 2>&1')
end, {nargs=1, complete='file'})

vim.api.nvim_create_user_command('Readable', function(table)
	if table.args ~= 'yes' then return print('Yes?') end
	vim.cmd.runtime('ftplugin/readable.vim')
end, {nargs=1})

vim.api.nvim_create_user_command('SwapDelete', function(table)
	if table.args ~= 'confirm' then
		print('Confirm?')
		return
	end
	local ok, err = os.remove(vim.fn.swapname(vim.fn.bufnr()))
	if not ok then
		vim.notify(err, vim.log.levels.ERROR)
	end
end, {nargs=1})


-- Auto commands
vim.api.nvim_create_augroup('vimrc_general_config', {clear=true})
vim.api.nvim_create_autocmd({'BufLeave'}, {
	group='vimrc_general_config',
	pattern='*',
	callback=function() vim.cmd.stopinsert() end
})
vim.api.nvim_create_autocmd({'TermOpen'}, {
	group='vimrc_general_config',
	pattern='*',
	callback=function()
		vim.api.nvim_set_option_value('number', false, {scope='local', win=vim.api.nvim_get_current_win()})
		ecall(vim.cmd.startinsert)
	end
})

vim.api.nvim_create_augroup('vimrc_filetype', {clear=true})
vim.api.nvim_create_autocmd({'BufNewFile', 'BufRead'}, {
	group='vimrc_filetype',
	pattern={'*.letter', '*.email'},
	callback=function() vim.cmd.setfiletype('readable') end
})


-- Spelling
vim.opt.spelllang = {'en_us', 'ru'}
-- vim.o.spellfile = vim.fn.stdpath('config') .. '/spell/filemanager.utf-8.add'
vim.o.spellfile = vim.fn.stdpath('config') .. '/spell/custom.utf-8.add'
vim.o.encoding = 'utf-8'


-- Plugin configuration
vim.g.tex_flavor = 'latex'

vim.g.forest_notifyoffilters = false
vim.g.forest_sortrules = {
	['/figures$'] = 'obey:*/,\\.tex$,*,.*/,.*',
	['/Downloads$'] = 'time'
}
vim.g.forest_alwaysexternal = '\\.pdf$'
vim.g.forest_uselscolors = true


-- Color scheme
vim.cmd.colorscheme('tuning')

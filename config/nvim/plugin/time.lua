-- A tiny plugin for measuring performance of Vim commands and other stuff

local time


function time(reps, cmd)
	local s = vim.loop.hrtime()
	for _ = 1, reps do vim.cmd(cmd) end
	local e = vim.loop.hrtime()
	print(('%s took %f ms (%d repetitions)'):format(cmd, (e-s)/1e6/reps, reps))
end


vim.api.nvim_create_user_command('Time', function(table)
	time(table.count, table.args)
end, {count=200, nargs=1, bar=true})

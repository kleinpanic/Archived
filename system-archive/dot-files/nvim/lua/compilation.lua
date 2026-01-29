-- File: ~/.config/nvim/lua/compilation.lua

local M = {}
local exec_terminal_buf = nil -- Module-level variable to track the exec terminal buffer

-- Utility function for non-blocking notifications
local function notify(msg, hl)
    vim.api.nvim_echo({{msg, hl}}, true, {})
end

-- Function to compile and run the current file
function M.compile_and_run()
    -- Get the current file path
    local file = vim.fn.expand('%')
    
    -- Check if the current buffer is a directory
    if vim.fn.isdirectory(file) == 1 then
        notify("Cannot compile a directory. Please open a source file.", "ErrorMsg")
        return
    end

    -- Determine the filetype (c, cpp, asm, etc.)
    local filetype = vim.bo.filetype
    local compiler = ''
    local flags = ''

    -- Set compiler and flags based on filetype
    if filetype == 'c' then
        compiler = 'gcc'
        flags = '-lm -O3'
    elseif filetype == 'cpp' then
        compiler = 'g++'
        flags = '-lm -O3'
    elseif filetype == 'asm' then
        compiler = 'nasm'
        flags = '-f elf64'
    else
        notify("Unsupported file type: " .. filetype, "ErrorMsg")
        return
    end

    -- Get the base name of the file without extension
    local output_name = vim.fn.expand('%:t:r') -- %:t:r extracts the filename without extension

    if filetype == 'asm' then
        -- Assemble the asm file to object file
        local assemble_cmd = string.format('%s %s "%s" -o "%s.o"', compiler, flags, file, output_name)
        
        -- Notify the user about the assembly process
        notify("Assembling...", "MoreMsg")
        
        -- Execute the assemble command
        local assemble_output = vim.fn.system(assemble_cmd)
        local assemble_exit = vim.v.shell_error

        -- Check if assembly was successful
        if assemble_exit ~= 0 then
            notify("Assembly failed:\n" .. assemble_output, "ErrorMsg")
            -- Populate quickfix list with errors and warnings
            vim.fn.setqflist({}, ' ')
            for line in assemble_output:gmatch("[^\r\n]+") do
                if line:match("error") or line:match("warning") then
                    vim.fn.setqflist({}, 'a', { lines = { line } })
                end
            end
            vim.cmd('copen') -- Open the quickfix list to show errors
            return
        end

        -- Link the object file to create executable
        local link_cmd = string.format('ld "%s.o" -o "%s"', output_name, output_name)
        
        -- Notify the user about the linking process
        notify("Linking...", "MoreMsg")
        
        -- Execute the link command
        local link_output = vim.fn.system(link_cmd)
        local link_exit = vim.v.shell_error

        -- Check if linking was successful
        if link_exit ~= 0 then
            notify("Linking failed:\n" .. link_output, "ErrorMsg")
            -- Populate quickfix list with errors and warnings
            vim.fn.setqflist({}, ' ')
            for line in link_output:gmatch("[^\r\n]+") do
                if line:match("error") or line:match("warning") then
                    vim.fn.setqflist({}, 'a', { lines = { line } })
                end
            end
            vim.cmd('copen') -- Open the quickfix list to show errors
            return
        else
            notify("Compilation successful!", "MoreMsg")
        end

    else
        -- For C and C++, compile and link in one step
        local compile_cmd = string.format('%s %s "%s" -o "%s"', compiler, flags, file, output_name)
        
        -- Notify the user about the compilation process
        notify("Compiling...", "MoreMsg")
        
        -- Execute the compile command
        local compile_output = vim.fn.system(compile_cmd)
        local compile_exit = vim.v.shell_error

        -- Check if compilation was successful
        if compile_exit ~= 0 then
            notify("Compilation failed:\n" .. compile_output, "ErrorMsg")
            -- Populate quickfix list with errors and warnings
            vim.fn.setqflist({}, ' ')
            for line in compile_output:gmatch("[^\r\n]+") do
                if line:match("error") or line:match("warning") then
                    vim.fn.setqflist({}, 'a', { lines = { line } })
                end
            end
            vim.cmd('copen') -- Open the quickfix list to show errors
            return
        else
            notify("Compilation successful!", "MoreMsg")
        end
    end

    -- Construct the run command
    local run_cmd = string.format('./%s', output_name)
    
    -- Notify the user about the execution process
    notify("Running executable...", "MoreMsg")

    -- Function to find an existing terminal window for execution
    local function find_exec_terminal()
        for _, win in ipairs(vim.api.nvim_list_wins()) do
            local buf = vim.api.nvim_win_get_buf(win)
            if vim.api.nvim_buf_get_option(buf, 'buftype') == 'terminal' then
                return win
            end
        end
        return nil
    end

    -- Run the executable in a terminal split at the bottom
    local exec_win = find_exec_terminal()

    if not exec_win then
        -- Open a new terminal split at the bottom
        vim.cmd('botright split')
        vim.cmd('resize 15')            -- Optional: Adjust the size of the split
        vim.cmd('terminal ' .. run_cmd) -- Run the executable
    else
        -- If a terminal is already open, send the run command to it
        vim.api.nvim_set_current_win(exec_win)
        vim.api.nvim_chan_send(vim.b.terminal_job_id, run_cmd .. '\n')
    end
end

-- Function to set up keybindings
function M.setup()
    -- Bind <leader>c to compile_and_run in normal mode
    vim.keymap.set('n', '<leader>c', M.compile_and_run, { noremap = true, silent = true })
end

return M


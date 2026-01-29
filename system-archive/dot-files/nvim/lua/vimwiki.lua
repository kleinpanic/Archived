-- Vimwiki Configuration
vim.cmd([[
    set nocompatible
    filetype plugin on
    syntax on
]])

vim.g.vimwiki_list = {
    {
        path = '~/vimwiki/',
        syntax = 'markdown',
        ext = '.md',
	diary_rel_path = 'Diary/'
    }
}

local M = {}

-- Create a LaTeX file with a more advanced template
function M.create_latex()
    local markdown_path = vim.fn.expand('%:p') -- Full path of the current markdown file
    local tex_name = vim.fn.expand('%:t:r') .. ".tex" -- Base name with .tex extension
    local tex_path = markdown_path:gsub('/[^/]*$', '') .. "/" .. tex_name -- Path to the new .tex file

    -- Check if the .tex file already exists
    if vim.fn.filereadable(tex_path) == 0 then
        -- Define the LaTeX template as a table of lines
        local template = {
            "\\documentclass[12pt]{article}",
            "\\usepackage{amsmath, amssymb, amsthm}",
            "\\usepackage{graphicx}", -- For including images
            "\\usepackage{hyperref}", -- For clickable links
            "\\usepackage[margin=1in]{geometry}", -- Page margins
            "\\usepackage{enumitem}", -- Better lists
            "",
            "\\title{Document Title}",
            "\\author{Author Name}",
            "\\date{\\today}",
            "",
            "\\begin{document}",
            "",
            "\\maketitle",
            "",
            "\\tableofcontents",
            "\\newpage",
            "",
            "\\section{Introduction}",
            "This is the introduction section.",
            "",
            "\\section{Main Content}",
            "This is where the main content goes.",
            "",
            "\\section{Conclusion}",
            "This is the conclusion.",
            "",
            "\\end{document}"
        }

        -- Write the template to the .tex file
        local success, err = pcall(function()
            vim.fn.writefile(template, tex_path)
        end)

        if not success then
            print("Error writing LaTeX template to file: " .. err)
            return
        end
    end

    -- Open the newly created .tex file
    vim.cmd('edit ' .. tex_path)
end

-- Delete key: Save the .tex file and return to the markdown file
function M.delete_latex()
    local tex_path = vim.fn.expand('%:p') -- Get the current LaTeX file path
    local markdown_path = tex_path:gsub('%.tex$', '.md') -- Replace .tex with .md

    vim.cmd('write') -- Save the LaTeX file
    vim.cmd('edit ' .. markdown_path) -- Open the corresponding markdown file
end

local zathura_pid = nil -- Global variable to store Zathura's actual PID

-- Helper function to get the PID of Zathura
local function get_zathura_pid(pdf_path)
    local cmd = string.format("pgrep -f 'zathura %s'", pdf_path)
    local pid = vim.fn.system(cmd):gsub("%s+", "") -- Get PID and trim whitespace
    if pid == "" then
        return nil
    end
    return tonumber(pid)
end

-- Stop monitoring and kill Zathura process
function M.stop_monitoring()
    vim.cmd([[
        augroup LatexLiveMonitor
            autocmd!
        augroup END
    ]])

    if zathura_pid ~= nil then
        print("Stopping Zathura (PID: " .. zathura_pid .. ")...")
        local result = vim.fn.system({'kill', '-9', tostring(zathura_pid)})
        if result ~= "" then
            print("Zathura process stopped.")
        else
            print("Failed to kill Zathura process.")
        end
        zathura_pid = nil
    else
        print("No Zathura process to stop.")
    end
end

-- Compile LaTeX and handle PDF
function M.compile_latex()
    local tex_path = vim.fn.expand('%:p') -- Full path of the current .tex file

    -- Ensure the current file is a .tex file
    if not tex_path:match("%.tex$") then
        print("This command can only be run inside a .tex file!")
        return
    end

    -- Save the .tex file
    vim.cmd('write')

    -- Define paths
    local pdf_name = vim.fn.expand('%:t:r') .. ".pdf" -- PDF file name
    local root_vimwiki = vim.fn.expand(vim.g.vimwiki_list[1].path) -- Expand ~ to home directory
    local supporting_files_path = root_vimwiki .. "/SupportingFiles/" -- SupportingFiles directory
    local pdf_dest = supporting_files_path .. pdf_name -- Full destination path for the PDF

    -- Ensure SupportingFiles directory exists
    if vim.fn.isdirectory(supporting_files_path) == 0 then
        vim.fn.mkdir(supporting_files_path, "p")
    end

    -- Compile LaTeX
    local compile_cmd = 'pdflatex -output-directory=' .. vim.fn.shellescape(vim.fn.expand('%:p:h')) .. ' ' .. vim.fn.shellescape(tex_path)
    vim.fn.system(compile_cmd)

    -- Move the generated PDF to SupportingFiles
    local compiled_pdf_path = vim.fn.expand('%:p:h') .. '/' .. pdf_name
    if vim.fn.filereadable(compiled_pdf_path) == 1 then
        local move_cmd = 'mv ' .. vim.fn.shellescape(compiled_pdf_path) .. ' ' .. vim.fn.shellescape(pdf_dest)
        vim.fn.system(move_cmd)
    else
        print("Error: PDF file was not generated.")
        return
    end

    -- Stop existing Zathura process if running
    if zathura_pid ~= nil then
        print("Stopping existing Zathura (PID: " .. zathura_pid .. ")...")
        vim.fn.system({'kill', '-9', tostring(zathura_pid)})
        zathura_pid = nil
    end

    -- Start Zathura and store its actual PID
    vim.fn.jobstart({'zathura', pdf_dest}, {detach = true})
    zathura_pid = get_zathura_pid(pdf_dest) -- Get the actual PID of Zathura
    if zathura_pid == nil then
        print("Failed to retrieve Zathura PID.")
        return
    end

    print("Zathura started (PID: " .. zathura_pid .. ").")

    -- Cleanup auxiliary files
    M.cleanup_auxiliary_files()

    -- Provide feedback to the user
    print("LaTeX compiled successfully. PDF moved to SupportingFiles.")
end

-- Stop Zathura when exiting Neovim or closing the buffer
vim.cmd([[
    augroup CloseZathuraOnExit
        autocmd!
        autocmd VimLeavePre * lua require('vimwiki').stop_monitoring()
        autocmd BufWinLeave,BufDelete *.tex lua require('vimwiki').stop_monitoring()
    augroup END
]])

-- Monitor file and recompile on Enter or `.`
function M.monitor_latex()
    local tex_path = vim.fn.expand('%:p') -- Full path of the current .tex file

    -- Ensure the current file is a .tex file
    if not tex_path:match("%.tex$") then
        print("This command can only be run inside a .tex file!")
        return
    end

    -- Run the initial compile (no Zathura is opened here)
    M.compile_latex_no_zathura()

    -- Define paths
    local pdf_name = vim.fn.expand('%:t:r') .. ".pdf" -- PDF file name
    local root_vimwiki = vim.fn.expand(vim.g.vimwiki_list[1].path) -- Expand ~ to home directory
    local supporting_files_path = root_vimwiki .. "/SupportingFiles/" -- SupportingFiles directory
    local pdf_dest = supporting_files_path .. pdf_name -- Full destination path for the PDF

    -- Ensure the PDF exists before starting monitoring
    if vim.fn.filereadable(pdf_dest) == 0 then
        print("Error: PDF file was not generated. Cannot start monitoring.")
        return
    end

    -- Start Zathura and store its actual PID
    if zathura_pid ~= nil then
        vim.fn.system({'kill', '-9', tostring(zathura_pid)}) -- Kill the old Zathura process
    end
    vim.fn.jobstart({'zathura', pdf_dest}, {detach = true})
    zathura_pid = get_zathura_pid(pdf_dest) -- Get the actual PID of Zathura
    if zathura_pid == nil then
        print("Failed to retrieve Zathura PID.")
        return
    end
    print("Zathura started (PID: " .. zathura_pid .. ").")

    -- Enter live monitoring mode
    vim.cmd([[
        augroup LatexLiveMonitor
            autocmd!
            " Trigger compilation on save
            autocmd BufWritePost <buffer> lua require('vimwiki').compile_latex_no_zathura()
            " Stop monitoring when the file is closed or buffer is unloaded
            autocmd BufWinLeave,BufDelete <buffer> lua require('vimwiki').stop_monitoring()
        augroup END
    ]])

    -- Map Enter and . to save the file
    vim.api.nvim_buf_set_keymap(0, 'i', '<CR>', '<CR><Esc>:w<CR>a', { noremap = true, silent = true })
    vim.api.nvim_buf_set_keymap(0, 'i', '.', '.<Esc>:w<CR>a', { noremap = true, silent = true })

    print("Live monitoring started. PDF will update on save.")
end

-- Compile LaTeX without opening Zathura
function M.compile_latex_no_zathura()
    local tex_path = vim.fn.expand('%:p') -- Full path of the current .tex file

    -- Ensure the current file is a .tex file
    if not tex_path:match("%.tex$") then
        return
    end

    -- Save the .tex file
    vim.cmd('write')

    -- Define paths
    local pdf_name = vim.fn.expand('%:t:r') .. ".pdf" -- PDF file name
    local root_vimwiki = vim.fn.expand(vim.g.vimwiki_list[1].path) -- Expand ~ to home directory
    local supporting_files_path = root_vimwiki .. "/SupportingFiles/" -- SupportingFiles directory
    local pdf_dest = supporting_files_path .. pdf_name -- Full destination path for the PDF

    -- Compile LaTeX
    local compile_cmd = 'pdflatex -output-directory=' .. vim.fn.shellescape(vim.fn.expand('%:p:h')) .. ' ' .. vim.fn.shellescape(tex_path)
    vim.fn.system(compile_cmd)

    -- Move the generated PDF to SupportingFiles
    local compiled_pdf_path = vim.fn.expand('%:p:h') .. '/' .. pdf_name
    if vim.fn.filereadable(compiled_pdf_path) == 1 then
        vim.fn.system('mv ' .. vim.fn.shellescape(compiled_pdf_path) .. ' ' .. vim.fn.shellescape(pdf_dest))
    else
        print("Error: PDF file was not generated.")
        return
    end

    -- Cleanup auxiliary files
    M.cleanup_auxiliary_files()
end

-- Cleanup auxiliary files
function M.cleanup_auxiliary_files()
    local tex_path = vim.fn.expand('%:p') -- Full path of the current .tex file
    local tex_dir = vim.fn.expand('%:p:h') -- Directory containing the .tex file
    local base_name = vim.fn.expand('%:t:r') -- Base name of the .tex file

    -- Auxiliary files to clean
    local aux_files = {
        tex_dir .. "/" .. base_name .. ".log",
        tex_dir .. "/" .. base_name .. ".aux",
        tex_dir .. "/texput.log"
    }

    for _, file in ipairs(aux_files) do
        if vim.fn.filereadable(file) == 1 then
            vim.fn.delete(file)
        end
    end
end

-- Link PDF in the markdown file
function M.link_pdf()
    local pdf_path = vim.fn.getreg('a') -- Get the PDF path from register "a"
    local link = "[" .. vim.fn.expand('%:t:r') .. "](" .. pdf_path .. ")"
    vim.fn.append(vim.fn.line('.'), link) -- Insert the link below the current line
    print("PDF linked in markdown!")
end

-- Keybindings
vim.api.nvim_set_keymap('n', '<leader>lt', [[:lua require('vimwiki').create_latex()<CR>]], { noremap = true, silent = true })
vim.api.nvim_set_keymap('n', '<leader>ld', [[:lua require('vimwiki').stop_monitoring()<CR>:lua require('vimwiki').delete_latex()<CR>]], { noremap = true, silent = true })
vim.api.nvim_set_keymap('n', '<leader>lc', [[:lua require('vimwiki').compile_latex()<CR>]], { noremap = true, silent = true })
vim.api.nvim_set_keymap('n', '<leader>lm', [[:lua require('vimwiki').monitor_latex()<CR>]], { noremap = true, silent = true })
vim.api.nvim_set_keymap('n', '<leader>ll', [[:lua require('vimwiki').link_pdf()<CR>]], { noremap = true, silent = true })

return M


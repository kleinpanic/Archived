--local map = vim.api.nvim_set_keymap
local map = vim.keymap.set
local opts = { noremap = true, silent = true }

-- Telescope Keybindings
map('n', '<leader>ff', "<cmd>lua require('telescope.builtin').find_files()<CR>", opts)
map('n', '<leader>fg', "<cmd>lua require('telescope.builtin').live_grep()<CR>", opts)
map('n', '<leader>fb', "<cmd>lua require('telescope.builtin').buffers()<CR>", opts)
map('n', '<leader>fh', "<cmd>lua require('telescope.builtin').help_tags()<CR>", opts)

-- Language Server Protocol (LSP) Keybindings
-- gD: Go to the declaration of the symbol under the cursor.
-- gd: Go to the definition of the symbol under the cursor.
-- K: Show documentation for the symbol under the cursor.
-- gi: Go to the implementation of a symbol (e.g., the implementation of an interface or function).
-- <C-k>: Show the function signature while coding.\
-- <space>rn: Rename a symbol (e.g., a variable or function).
-- <space>ca: Trigger a code action (e.g., auto-fix issues, refactor code).
-- gr: List all references to the symbol under the cursor.
-- <space>f: Format the current buffer using the language server.
local function lsp_keybindings(bufnr)
    local opts = { noremap = true, silent = true, buffer = bufnr }

    vim.keymap.set('n', 'gD', vim.lsp.buf.declaration, opts)
    vim.keymap.set('n', 'gd', vim.lsp.buf.definition, opts)
    vim.keymap.set('n', 'K', vim.lsp.buf.hover, opts)
    vim.keymap.set('n', 'gi', vim.lsp.buf.implementation, opts)
    vim.keymap.set('n', '<C-k>', vim.lsp.buf.signature_help, opts)
    vim.keymap.set('n', '<space>rn', vim.lsp.buf.rename, opts)
    vim.keymap.set('n', '<space>ca', vim.lsp.buf.code_action, opts)
    vim.keymap.set('n', 'gr', vim.lsp.buf.references, opts)
    vim.keymap.set('n', '<space>f', vim.lsp.buf.format, opts)
end

-- Wikipedia search
function SearchWikipedia()
    local query = vim.fn.input('Wikipedia Search: ')
    query = vim.fn.shellescape(query)
    local search_url = "https://en.wikipedia.org/wiki/Special:Search?search=" .. query
    vim.cmd('silent !xdg-open ' .. search_url)
end
vim.api.nvim_set_keymap('n', '<leader>w', ':lua SearchWikipedia()<CR>', { noremap = true, silent = true })


-- Vimwiki Markdown Preview
function VimwikiMarkdownPreview()
    -- Path to the script
    local script_path = vim.fn.expand('~/.config/nvim/scripts/vimwiki-markdown-preview.sh')
    -- Check if the script exists
    if vim.fn.filereadable(script_path) == 0 then
        vim.notify("Script not found at " .. script_path, vim.log.levels.ERROR)
        return
    end
    -- Run the script with --index-wiki flag
    local command = string.format('bash %s --index-wiki', vim.fn.shellescape(script_path))
    vim.cmd('silent !' .. command)
end
map('n', '<leader>mip', VimwikiMarkdownPreview, opts)

-- Vimwiki Convert Current File to HTML, move it, and open with qutebrowser
function VimwikiConvertCurrent()
    -- Path to the script
    local script_path = vim.fn.expand('~/.config/nvim/scripts/vimwiki-markdown-preview.sh')
    -- Check if the script exists
    if vim.fn.filereadable(script_path) == 0 then
        vim.notify("Script not found at " .. script_path, vim.log.levels.ERROR)
        return
    end
    -- Get the current file path
    local current_file = vim.api.nvim_buf_get_name(0)
    -- vim.notify("Retrieved file path: " .. current_file, vim.log.levels.INFO) -- Debugging: Print the file path
    -- Check if it's a markdown file
    if not current_file:match('%.md$') then
        vim.notify('Current file is not a Markdown file.', vim.log.levels.ERROR)
        return
    end
    -- Check if the source file exists
    if vim.fn.filereadable(current_file) == 0 then
        vim.notify('Current Markdown file does not exist.', vim.log.levels.ERROR)
        return
    end
    -- Explicitly construct the command string
    local command = "bash " .. vim.fn.shellescape(script_path) .. " --convert " .. current_file
    vim.notify("Running command: " .. command, vim.log.levels.INFO) -- Debugging: Print the command being run
    -- Run the command
    vim.cmd('silent !' .. command)
    vim.notify('Conversion and opening in qutebrowser completed.', vim.log.levels.INFO)
end

map('n', '<leader>mp', VimwikiConvertCurrent, opts)

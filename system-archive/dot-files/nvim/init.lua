-- Load settings
require('settings')

-- Load plugins
require('plugins')

-- Load keybindings
require('keybindings')

-- Load Vimwiki and LaTeX integration
require('vimwiki')

-- Load Compilation Integrations:
require('compilation').setup()

-- Load and setup Encrypted File Handler. 
require("encrypted_file_handler").setup({
  prompt_method = "vim_ui",  -- change to "dmenu" or "terminal" if desired
  decryption_methods = {
    -- Optionally, define custom methods for non-standard extensions:
    -- vault = function(pass, orig, tmp)
    --   return string.format("my_custom_decrypt -i %s -o %s -p %s", vim.fn.shellescape(orig), vim.fn.shellescape(tmp), vim.fn.shellescape(pass))
    -- end,
  },
  cleanup_timeout = 0,  -- set a timeout (in seconds) if you prefer auto-deletion after some time
  debug = false,
})


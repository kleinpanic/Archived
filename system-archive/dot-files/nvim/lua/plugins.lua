-- treesitter
require'nvim-treesitter.configs'.setup{
    highlight = {
        enable = true,
    },
    indent = {
        enable = true,
    }
}

-- telescope
local builtin = require('telescope.builtin')

-- lualine
require('lualine').setup()

-- nvim-lint
require('lint').linters_by_ft = {
	markdown = {'value'},
}

-- comment
require('Comment').setup()


-- autopairs
require('nvim-autopairs').setup()


-- cmp
local cmp = require'cmp'
cmp.setup({
	snippet = {
		expand = function(args)
			require('luasnip').lsp_expand(args.body)
		end
	}
})
cmp.setup.cmdline(':', {
   mapping = cmp.mapping.preset.cmdline(),
   sources = cmp.config.sources({
     { name = 'path' }
   }, {
     { name = 'cmdline' }
   }),
   matching = { disallow_symbol_nonprefix_matching = false }
})
-- TODO complete https://github.com/hrsh7th/nvim-cmp

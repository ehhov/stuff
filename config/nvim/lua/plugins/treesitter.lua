-- ~/.config/nvim/lua/plugins/treesitter.lua

return {
  "nvim-treesitter/nvim-treesitter",
  build = ":TSUpdate", -- This command runs automatically after installation/update
  event = { "BufReadPre", "BufNewFile" },
  dependencies = {
    "nvim-treesitter/nvim-treesitter-textobjects",
  },
  
  -- The configuration for nvim-treesitter itself
  opts = {
    -- A list of parser names to install
    -- You can add more languages here as you need them
    ensure_installed = {
      "vim",
      "lua",
      "javascript",
      "typescript",
      "html",
      "css",
      "json",
      "bash",
      "markdown",
      "markdown_inline",
      "python",
      "rust",
      "c",
      "cpp",
      "make",
    },

    -- Enable these features
    highlight = {
      enable = true, -- Enable syntax highlighting
      -- Use `false` for slower machines to improve performance
      -- disable = { "c", "rust" },
      -- Setting this to `true` will use treesitter for all filetypes with a parser.
      -- This is the recommended setting and the default.
      additional_vim_regex_highlighting = true,
    },
    
    -- Other Treesitter features (optional)
    indent = {
        enable = true,
        disable = { "python" },
    }, -- Enable indentation based on treesitter
    
    textobjects = {
      select = {
        enable = true,
        lookahead = true, -- lookahead for textobjects
        keymaps = {
          -- Example keymaps for textobjects. You can customize these.
          ["af"] = "@function.outer",
          ["if"] = "@function.inner",
          ["ac"] = "@class.outer",
          ["ic"] = "@class.inner",
        },
      },
      swap = {
        enable = true,
        swap_next = {
          ["<leader>a"] = "@parameter.inner",
        },
        swap_previous = {
          ["<leader>A"] = "@parameter.inner",
        },
      },
    },
  },
  
  -- This is the `config` function. It runs after the plugin is loaded.
  config = function(_, opts)
    -- This command ensures that all the parsers you listed above are installed.
    -- It's often good practice to put this inside a build or post-install script.
    require("nvim-treesitter.configs").setup(opts)
  end,
}

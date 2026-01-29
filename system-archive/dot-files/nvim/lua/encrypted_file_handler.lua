-- File: lua/encrypted_file_handler.lua
local M = {}

-- Default configuration; users can override these via setup()
local config = {
  -- Patterns for encrypted files (Lua patterns, so escape the dot)
  file_patterns = { "%.pgp$", "%.gpg$", "%.enc$", "%.aes$", "%.kdbx$", "%.vault$" },
  -- Passphrase prompt method: "vim_ui", "dmenu", or "terminal"
  prompt_method = "vim_ui",
  -- Custom decryption methods by extension; leave empty to use builtins
  -- e.g., config.decryption_methods["kdbx"] = function(pass, orig, tmp) ... end
  decryption_methods = {},
  -- Automatically delete decrypted temp file when buffer is closed
  auto_cleanup = true,
  -- Optional timeout (in seconds) to auto-delete decrypted file (0 = disabled)
  cleanup_timeout = 0,
  -- Debug flag (prints extra info)
  debug = false,
}

-- Merge user config with defaults
function M.setup(user_config)
  config = vim.tbl_extend("force", config, user_config or {})
  if config.debug then
    print("Encrypted file handler loaded with config:")
    print(vim.inspect(config))
  end
  M.setup_autocmds()
end

-- Check if the filename matches any encrypted file pattern
local function is_encrypted_file(filename)
  for _, pattern in ipairs(config.file_patterns) do
    if filename:match(pattern) then
      return true
    end
  end
  return false
end

-- Prompt the user for a passphrase via different methods.
-- The callback is called with the entered passphrase.
local function prompt_passphrase(callback)
  if config.prompt_method == "vim_ui" then
    vim.ui.input({ prompt = "Enter decryption passphrase: ", secret = true }, function(input)
      callback(input)
    end)
  elseif config.prompt_method == "dmenu" then
    -- dmenu: call it synchronously via io.popen
    local handle = io.popen("dmenu -p 'Enter decryption passphrase:'")
    local result = handle:read("*a") or ""
    handle:close()
    result = result:gsub("\n", "") -- trim newline
    callback(result)
  elseif config.prompt_method == "terminal" then
    -- Terminal: create a temporary floating window to ask for input.
    local bufnr = vim.api.nvim_create_buf(false, true)
    local width = 50
    local height = 1
    local opts = {
      relative = "editor",
      width = width,
      height = height,
      row = (vim.o.lines - height) / 2,
      col = (vim.o.columns - width) / 2,
      border = "single",
    }
    local win = vim.api.nvim_open_win(bufnr, true, opts)
    vim.api.nvim_buf_set_lines(bufnr, 0, -1, false, { "Enter decryption passphrase:" })
    -- Fallback: use vim.fn.input for capturing input (note: this does not hide the input)
    local result = vim.fn.input("Passphrase: ")
    vim.api.nvim_buf_delete(bufnr, { force = true })
    callback(result)
  else
    -- Default fallback: vim.ui.input
    vim.ui.input({ prompt = "Enter decryption passphrase: ", secret = true }, function(input)
      callback(input)
    end)
  end
end

-- Built-in decryption command generators
local builtin_methods = {
  pgp = function(pass, orig, tmp)
    -- GPG: passphrase is fed via stdin using --passphrase-fd 0
    return string.format("gpg --batch --yes --passphrase-fd 0 --output %s --decrypt %s", 
      vim.fn.shellescape(tmp), vim.fn.shellescape(orig))
  end,
  gpg = function(pass, orig, tmp)
    return string.format("gpg --batch --yes --passphrase-fd 0 --output %s --decrypt %s", 
      vim.fn.shellescape(tmp), vim.fn.shellescape(orig))
  end,
  enc = function(pass, orig, tmp)
    -- OpenSSL: passphrase is provided inline (note: this exposes it in the process list)
    return string.format("openssl enc -aes-256-cbc -d -in %s -out %s -pass pass:%s", 
      vim.fn.shellescape(orig), vim.fn.shellescape(tmp), vim.fn.shellescape(pass))
  end,
  aes = function(pass, orig, tmp)
    return string.format("openssl enc -aes-256-cbc -d -in %s -out %s -pass pass:%s", 
      vim.fn.shellescape(orig), vim.fn.shellescape(tmp), vim.fn.shellescape(pass))
  end,
}

-- Determines which decryption command to use based on the file extension.
-- Returns the command string or nil plus an error message.
local function get_decryption_command(ext, pass, orig, tmp)
  ext = ext:lower()
  local method = nil
  if ext == "pgp" or ext == "gpg" then
    method = config.decryption_methods[ext] or builtin_methods.gpg
  elseif ext == "enc" or ext == "aes" then
    method = config.decryption_methods[ext] or builtin_methods.enc
  else
    -- For custom types (e.g. kdbx, vault), require a user-supplied method.
    method = config.decryption_methods[ext]
    if not method then
      return nil, "No decryption method defined for extension: " .. ext
    end
  end
  if type(method) == "function" then
    return method(pass, orig, tmp)
  elseif type(method) == "string" then
    return string.format(method, vim.fn.shellescape(tmp), vim.fn.shellescape(orig), vim.fn.shellescape(pass))
  else
    return nil, "Invalid decryption method configuration for extension: " .. ext
  end
end

-- Decrypts the file and opens the decrypted content in a new, secure buffer.
function M.decrypt_and_open(original_file)
  local ext = original_file:match("^.+%.([^.]+)$")
  if not ext then
    print("Unable to determine file extension for decryption.")
    return
  end

  prompt_passphrase(function(passphrase)
    if not passphrase or passphrase == "" then
      print("Decryption cancelled: No passphrase provided.")
      return
    end

    local tmp_file = vim.fn.tempname() -- temporary file path
    local cmd, err = get_decryption_command(ext, passphrase, original_file, tmp_file)
    if not cmd then
      print("Decryption error: " .. err)
      return
    end

    if config.debug then
      print("Running decryption command: " .. cmd)
    end

    -- For GPG, we need to feed the passphrase via stdin.
    local stdin = (ext == "pgp" or ext == "gpg") and passphrase or nil
    local output = vim.fn.system(cmd, stdin)
    local exit_code = vim.v.shell_error
    if exit_code ~= 0 then
      print("Decryption failed. Please check your passphrase and method.")
      if config.debug then
        print("Command output: " .. output)
      end
      return
    end

    -- Open the decrypted file in a new buffer
    vim.cmd("edit " .. vim.fn.fnameescape(tmp_file))
    local bufnr = vim.api.nvim_get_current_buf()
    -- Set buffer options: nofile, no swapfile, readonly
    vim.bo[bufnr].buftype = "nofile"
    vim.bo[bufnr].swapfile = false
    vim.bo[bufnr].readonly = true

    -- Store the original encrypted file and temp file paths in buffer variables
    vim.api.nvim_buf_set_var(bufnr, "original_encrypted_file", original_file)
    vim.api.nvim_buf_set_var(bufnr, "decrypted_temp_file", tmp_file)

    -- Setup automatic cleanup when the buffer is unloaded or wiped out
    if config.auto_cleanup then
      vim.api.nvim_create_autocmd({ "BufUnload", "BufWipeout" }, {
        buffer = bufnr,
        callback = function()
          if vim.loop.fs_stat(tmp_file) then
            os.remove(tmp_file)
            if config.debug then
              print("Cleaned up temporary decrypted file: " .. tmp_file)
            end
          end
        end,
      })
      -- Optionally, add a timer to auto-delete the file after a set timeout
      if config.cleanup_timeout and config.cleanup_timeout > 0 then
        vim.defer_fn(function()
          if vim.loop.fs_stat(tmp_file) then
            os.remove(tmp_file)
            if config.debug then
              print("Auto-cleanup: Deleted temporary decrypted file after timeout: " .. tmp_file)
            end
          end
        end, config.cleanup_timeout * 1000)
      end
    end
  end)
end

-- Autocommand callback: if the file being opened matches an encrypted pattern,
-- schedule decryption.
local function on_buf_read_pre(args)
  local filename = args.file
  if filename and is_encrypted_file(filename) then
    vim.schedule(function()
      M.decrypt_and_open(filename)
    end)
  end
end

-- Setup autocommands to hook into BufReadPre and BufNewFile events.
function M.setup_autocmds()
  vim.api.nvim_create_autocmd({ "BufReadPre", "BufNewFile" }, {
    callback = on_buf_read_pre,
  })
end

-- Expose a user command to manually trigger decryption.
vim.api.nvim_create_user_command("DecryptFile", function(opts)
  local file = opts.args
  if file == "" then
    file = vim.fn.expand("%:p")
  end
  M.decrypt_and_open(file)
end, { nargs = "?" })

return M


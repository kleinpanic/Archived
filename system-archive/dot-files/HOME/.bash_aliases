# lsd as ls
if [ "$TERM" != "linux" ] && [ "$TERM" != "tmux-256color" ] && [ "$TERM" != "xterm-256color" ]; then
    alias ls='lsd'
    alias la='lsd -A'
    alias lt='lsd --tree'
    alias ll='lsd -l'
else
    alias ls='ls --color=auto'
    alias la='ls -A'
    alias ll='ls -l'
fi 
# Handy shit
alias py='python3'
alias pyvenv='python3 -m venv'
alias ..='cd ..'
alias cdir='cd "${_%/*}"'
alias reboot='sudo reboot'

alias untar='tar -xzf'

# Remove files and directories with confirmation
alias rm='rm -i'

# Git shortcuts
alias gst='git status'
alias gco='git checkout'
alias gl='git pull'
alias gp='git push'

#vim as nvim
alias vim='nvim'
alias vi='nvim'
alias nvi='nvim'

# Create a new directory and navigate into it
alias mkcd='foo(){ mkdir -p "$1"; cd "$1"; }; foo'

# Search for a process
alias psg='ps aux | grep -v grep | grep -i -e VSZ -e'

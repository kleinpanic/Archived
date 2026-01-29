# .config Directory

This repository contains my personal configuration files for various applications and scripts. The `.gitignore` is set up to ignore everything by default, except for explicitly whitelisted directories and files. Below is an overview of what is included.

## Included Configurations

### **General Configuration Files**
- `libinput-gestures.conf` - Configuration for touchpad gestures.
- `daemon_starter.conf` - My personal daemon starter configuration.
- `README.md` - This documentation.
- `LICENSE` - Licensing information.

### **Applications & Programs**
#### **Neovim (`nvim/`)**
- Configuration files for Neovim, including settings, keybindings, and plugins.

#### **Tmux (`tmux/`)**
- Custom configurations for Tmux terminal multiplexer.

#### **Conky (`conky/`)**
- Configuration for Conky, a system monitoring tool.

#### **Skippy-XD (`skippy-xd/`)**
- Configurations for Skippy-XD, a full-screen task switcher.

#### **Qutebrowser (`qutebrowser/`)**
- Custom settings for Qutebrowser, my preferred minimal web browser.

#### **Neofetch (`neofetch/`)**
- Custom ASCII and system info display configurations.

#### **Tox (`tox/`)**
- Configuration files for Tox, a secure P2P messaging client.

#### **Systemd (`systemd/`)**
- Custom systemd service configurations.

#### **LSD (`lsd/`)**
- Configuration for LSD, a modern `ls` replacement.

### **Personal Programs**
#### **Battery Monitor (`battery_monitor/`)**
- A personal program that monitors battery status and provides alerts.

#### **DailyApp (`dailyapp/`)**
- My personal daily application launcher and tracker.

#### **Daemon Starter (`daemon_starter.conf`)**
- A configuration file for my custom daemon starter program.

### **Setup (`setup/`)**
- Scripts and configuration files for setting up my environment.

### **Compositing (`picom/`)**
- Configuration files for `picom`, my X compositor setup.

### **HOME (`HOME/`)**
This directory includes specific home directory configuration files:
- `.bashrc`
- `.bash_aliases`
- `.xinitrc`
- `.profile`
- `.inputrc`

## Usage
Clone this repository and copy or symlink necessary configurations:
```sh
cd ~/.config
git clone --bare <repo_url> .
```
To restore configs, use:
```sh
git --git-dir=$HOME/.config --work-tree=$HOME checkout -f
```

## Notes
- This setup allows me to track specific configurations while keeping unnecessary clutter out of version control.
- Some of my personal scripts and programs are included here for convenience.
- Certain files require additional dependencies or custom scripts to function correctly.



#!/bin/sh

if [ "$1" = "remove" ]
then
    rm -rf build def.egg-info
    pipx uninstall def
else
    mkdir -p "$HOME/.local/share/def"
    cp -n "etc/dictionary.db" "$HOME/.local/share/def/dictionary.db"
    pipx install .
fi

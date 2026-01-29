#!/usr/bin/env bash

# Search for all possible boards, remove the header, and clean up whitespace lines
arduino-cli board search | tail -n +2 | sed '/^\s*$/d' > /tmp/arduino_boards.txt

# Ensure the file has the correct permissions for Lua to read
chmod 644 /tmp/arduino_boards.txt

# Print a message indicating that the process is complete
echo "All possible boards have been listed in /tmp/arduino_boards.txt"

#!/bin/zsh

# Check if script is being run with sudo
if [ "$EUID" -ne 0 ]; then
    echo "Please run this script with sudo."
    exit
fi

# Install necessary packages
apt-get update
apt-get install -y build-essential binutils gcc-arm-linux-gnueabi clang libfreetype6 nasm

# Notify user that installation is complete
echo "Installation completed successfully."



#!/bin/bash

# Check CMake installation
if command -v cmake &> /dev/null; then
    echo "CMake is installed, current version:"
    cmake --version
    exit 0
fi

# Update package list and install CMake
echo "Installing CMake..."
sudo apt-get update
sudo apt-get install -y cmake

# Verify installation
if command -v cmake &> /dev/null; then
    echo "CMake is installed successfully, version:"
    cmake --version
else
    echo "CMake installation failed, please check your network or install it manually."
    exit 1
fi
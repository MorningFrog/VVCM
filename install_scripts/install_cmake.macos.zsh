#!/bin/zsh

set -euo pipefail

# Add line to file if it doesn't already exist
add_line_if_missing() {
  local line="$1"
  local file="$2"
  grep -Fqs "$line" "$file" 2>/dev/null || echo "$line" >> "$file"
}

# Check CMake installation
if command -v cmake >/dev/null 2>&1; then
  echo "CMake is installed, current version:"
  cmake --version
  exit 0
fi

echo "CMake not found. Preparing to install on macOS..."

# Install CMake
echo "Installing CMake via Homebrew..."
brew install cmake

# Verify installation
if command -v cmake >/dev/null 2>&1; then
  echo "CMake installed successfully:"
  cmake --version
else
  echo "CMake installation failed, please check your network or install it manually."
  exit 1
fi
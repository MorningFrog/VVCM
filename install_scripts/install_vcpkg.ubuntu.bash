#!/bin/bash

###############################
# Install vcpkg script, Linux #
###############################

set -e  # Exit on error

# Set installation path (modifiable)
INSTALL_DIR="$HOME/vcpkg"

echo "Installing vcpkg to $INSTALL_DIR"

# Clone vcpkg
if [ ! -d "$INSTALL_DIR" ]; then
  git clone https://github.com/microsoft/vcpkg.git "$INSTALL_DIR"
else
  echo "vcpkg directory already exists. Pulling latest changes..."
  (cd "$INSTALL_DIR" && git pull --ff-only)
fi

# Bootstrap the installation
cd "$INSTALL_DIR"
./bootstrap-vcpkg.sh -disableMetrics

# Add line if missing function
add_line_if_missing () {
  local line="$1"
  local file="$2"
  grep -Fqs "$line" "$file" 2>/dev/null || echo "$line" >> "$file"
}

# Set VCPKG_ROOT
add_line_if_missing "export VCPKG_ROOT=\"$INSTALL_DIR\"" ~/.bashrc

# Add vcpkg to PATH
add_line_if_missing "export PATH=\"\$VCPKG_ROOT:\$PATH\"" ~/.bashrc

# Set CMake toolchain file for vcpkg
echo "Setting CMake toolchain file for vcpkg"
add_line_if_missing "export CMAKE_TOOLCHAIN_FILE=\"\$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake\"" ~/.bashrc
source ~/.bashrc


echo "vcpkg installed to $INSTALL_DIR"
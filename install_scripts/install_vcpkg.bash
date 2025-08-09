#!/bin/bash

###############################
# Install vcpkg script, Linux #
###############################

set -e  # Exit on error

# Set installation path (modifiable)
INSTALL_DIR="$HOME/vcpkg"

echo "Installing vcpkg to $INSTALL_DIR"

if [ ! -d "$INSTALL_DIR" ]; then
  git clone https://github.com/microsoft/vcpkg.git "$INSTALL_DIR"
fi

# Bootstrap the installation
cd "$INSTALL_DIR"
./bootstrap-vcpkg.sh -disableMetrics

# Set default to install release version of libraries
echo "set(VCPKG_BUILD_TYPE release)" | tee -a triplets/x64-linux.cmake

# Add vcpkg to PATH
echo "export PATH=\"$INSTALL_DIR:$PATH\"" >> ~/.bashrc
source ~/.bashrc

# Set CMake toolchain file for vcpkg
echo "Setting CMake toolchain file for vcpkg"
echo "export CMAKE_TOOLCHAIN_FILE=\"$INSTALL_DIR/scripts/buildsystems/vcpkg.cmake\"" >> ~/.bashrc
source ~/.bashrc


echo "vcpkg installed to $INSTALL_DIR"
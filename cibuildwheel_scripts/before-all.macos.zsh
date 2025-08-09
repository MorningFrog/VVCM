#!/usr/bin/env zsh
set -euo pipefail

# install prerequisites
command -v ninja >/dev/null 2>&1 || brew install ninja

# install vcpkg
VCPKG_ROOT="$HOME/vcpkg"

echo "Installing vcpkg to $VCPKG_ROOT"

# Clone vcpkg
if [ ! -d "$VCPKG_ROOT" ]; then
  git clone https://github.com/microsoft/vcpkg.git "$VCPKG_ROOT"
else
  echo "vcpkg directory already exists. Pulling latest changes..."
  (cd "$VCPKG_ROOT" && git pull --ff-only)
fi

# Bootstrap the installation
cd "$VCPKG_ROOT"
env -u CMAKE_TOOLCHAIN_FILE -u CMAKE_PREFIX_PATH -u CMAKE_MODULE_PATH \
  ./bootstrap-vcpkg.sh -disableMetrics

if [ "${ARCH}" = "arm64" ]; then
  DEFAULT_TRIPLET="arm64-osx"
else
  DEFAULT_TRIPLET="x64-osx"
fi

# install eigen3
"$VCPKG_ROOT/vcpkg" install "eigen3:${DEFAULT_TRIPLET}"
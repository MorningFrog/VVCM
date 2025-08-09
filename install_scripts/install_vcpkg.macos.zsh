#!/bin/zsh

################################
# Install vcpkg script, macOS  #
################################

set -euo pipefail

# Set installation path (modifiable)
INSTALL_DIR="$HOME/vcpkg"

echo "Installing vcpkg to $INSTALL_DIR"

# Clone vcpkg
if [ ! -d "${INSTALL_DIR}" ]; then
  git clone https://github.com/microsoft/vcpkg.git "${INSTALL_DIR}"
else
  echo "vcpkg directory already exists. Pulling latest changes..."
  (cd "${INSTALL_DIR}" && git pull --ff-only)
fi

# Bootstrap the installation
cd "$INSTALL_DIR"
./bootstrap-vcpkg.sh -disableMetrics

# Set default triplet based on architecture
ARCH="$(uname -m)"
if [ "${ARCH}" = "arm64" ]; then
  DEFAULT_TRIPLET="arm64-osx"
else
  DEFAULT_TRIPLET="x64-osx"
fi
echo "Detected architecture: ${ARCH} -> default triplet: ${DEFAULT_TRIPLET}"

# Set user-specific CMake configuration for vcpkg
USER_CMAKE="${INSTALL_DIR}/vcpkg-user-config.cmake"
{
  echo "set(VCPKG_BUILD_TYPE release)"
} > "${USER_CMAKE}"
echo "Wrote ${USER_CMAKE}"

# Add vcpkg to PATH (zsh)
add_line_if_missing () {
  local line="$1"
  local file="$2"
  grep -Fqs "$line" "$file" 2>/dev/null || echo "$line" >> "$file"
}
add_line_if_missing 'export VCPKG_ROOT="$HOME/vcpkg"'               "$HOME/.zshrc"
add_line_if_missing 'export PATH="$VCPKG_ROOT:$PATH"'               "$HOME/.zshrc"
add_line_if_missing 'export CMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"' "$HOME/.zshrc"
add_line_if_missing "export VCPKG_DEFAULT_TRIPLET=\"${DEFAULT_TRIPLET}\"" "$HOME/.zshrc"
add_line_if_missing 'export VCPKG_FEATURE_FLAGS="manifests,registries"'   "$HOME/.zshrc"
add_line_if_missing 'export VCPKG_ROOT="$HOME/vcpkg"'               "$HOME/.zprofile"
add_line_if_missing 'export PATH="$VCPKG_ROOT:$PATH"'               "$HOME/.zprofile"

echo "Environment variables appended to ~/.zshrc, ~/.zprofile."

# Source user profile
source "$HOME/.zprofile"
source "$HOME/.zshrc"

echo "vcpkg installed to $INSTALL_DIR"
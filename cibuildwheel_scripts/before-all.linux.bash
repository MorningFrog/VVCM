#!/usr/bin/env bash
set -eux

[ -n "${CMAKE_GENERATOR:-}" ] && echo "CMAKE_GENERATOR: $CMAKE_GENERATOR"
[ -n "${CMAKE_GENERATOR_PLATFORM:-}" ] && echo "CMAKE_GENERATOR_PLATFORM: $CMAKE_GENERATOR_PLATFORM"
[ -n "${VSCMD_ARG_TGT_ARCH:-}" ] && echo "VSCMD_ARG_TGT_ARCH: $VSCMD_ARG_TGT_ARCH"
[ -n "${VSINSTALLDIR:-}" ] && echo "VSINSTALLDIR: $VSINSTALLDIR"
[ -n "${VisualStudioVersion:-}" ] && echo "VisualStudioVersion: $VisualStudioVersion"
[ -n "${CMAKE_PREFIX_PATH:-}" ] && echo "CMAKE_PREFIX_PATH: $CMAKE_PREFIX_PATH"

# install prerequisites
if command -v yum >/dev/null 2>&1; then
    yum -y install git curl zip unzip tar gcc gcc-c++ ninja-build
elif command -v microdnf >/dev/null 2>&1; then
    microdnf -y install git curl zip unzip tar gcc gcc-c++ ninja-build
elif command -v apk >/dev/null 2>&1; then
    apk add --no-cache git curl zip unzip tar build-base ninja
fi

# install vcpkg
VCPKG_ROOT=/opt/vcpkg

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

echo "set(VCPKG_BUILD_TYPE release)" | tee -a triplets/${VCPKG_TARGET_TRIPLET:-x64-linux}.cmake

# install eigen3
"$VCPKG_ROOT/vcpkg" install "eigen3:${VCPKG_TARGET_TRIPLET}"
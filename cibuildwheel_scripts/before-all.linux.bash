set -eux

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
./bootstrap-vcpkg.sh -disableMetrics

echo "set(VCPKG_BUILD_TYPE release)" | tee -a triplets/${VCPKG_DEFAULT_TRIPLET:-x64-linux}.cmake

# install eigen3
"$VCPKG_ROOT/vcpkg" install Eigen3
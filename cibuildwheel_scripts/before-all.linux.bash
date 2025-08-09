set -eux

# install prerequisites
if command -v yum >/dev/null 2>&1; then
    yum -y install git curl zip unzip tar
elif command -v microdnf >/dev/null 2>&1; then
    microdnf -y install git curl zip unzip tar
elif command -v apk >/dev/null 2>&1; then
    apk add --no-cache git curl zip unzip tar
fi

# install vcpkg
INSTALL_DIR=/opt/vcpkg

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

echo "set(VCPKG_BUILD_TYPE release)" | tee -a triplets/${VCPKG_DEFAULT_TRIPLET:-x64-linux}.cmake

# install eigen3
"$INSTALL_DIR/vcpkg" install Eigen3
sudo apt update
sudo apt-get install -y pkg-config
sudo -E vcpkg install Eigen3
sudo apt-get install -y autoconf automake autoconf-archive
sudo -E vcpkg install pybind11
pip3 install pybind11-stubgen numpy
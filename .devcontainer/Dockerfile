FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    sudo \
    build-essential \
    git \
    cmake \
    ninja-build \
    python3 \
    python3-pip \
    curl zip unzip tar \
    pkg-config

RUN cd /usr/local && git clone https://github.com/microsoft/vcpkg \
    && cd vcpkg && ./bootstrap-vcpkg.sh -disableMetrics \
    && echo "set(VCPKG_BUILD_TYPE release)" | tee -a triplets/x64-linux.cmake \
    && ln -s /usr/local/vcpkg/vcpkg /usr/bin/vcpkg \
    && ./vcpkg integrate install

RUN echo "export CMAKE_TOOCHAIN_FILE=/usr/local/vcpkg/scripts/buildsystems/vcpkg.cmake" > ~/.bashrc \
    && echo "export CMAKE_PREFIX_PATH=/usr/local/vcpkg/installed/x64-linux" > ~/.bashrc
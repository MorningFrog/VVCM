FROM ubuntu:22.04

# Python 版本
ARG PYTHON_VERSION=3.10

# 设置环境变量为非交互式
ENV DEBIAN_FRONTEND=noninteractive

# 换源
RUN sed -i 's/http:\/\/archive.ubuntu.com/http:\/\/mirror.sjtu.edu.cn/g' /etc/apt/sources.list
RUN sed -i 's/http:\/\/ports.ubuntu.com/http:\/\/mirror.sjtu.edu.cn/g' /etc/apt/sources.list
RUN apt-get update && apt-get install -y --reinstall ca-certificates
RUN sed -i 's/http:\/\/mirror.sjtu.edu.cn/https:\/\/mirror.sjtu.edu.cn/g' /etc/apt/sources.list

RUN apt-get update && apt-get install -y \
    software-properties-common \
    sudo \
    build-essential \
    git \
    cmake \
    ninja-build \
    curl zip unzip tar \
    pkg-config \
    autoconf automake autoconf-archive

# 安装 vcpkg
ENV VCPKG_ROOT=/usr/local/vcpkg
RUN cd /usr/local && git clone https://github.com/microsoft/vcpkg \
    && cd vcpkg && ./bootstrap-vcpkg.sh -disableMetrics \
    && echo "set(VCPKG_BUILD_TYPE release)" | tee -a triplets/x64-linux.cmake \
    && ln -s /usr/local/vcpkg/vcpkg /usr/bin/vcpkg \
    && ./vcpkg integrate install

RUN echo "export CMAKE_TOOCHAIN_FILE=/usr/local/vcpkg/scripts/buildsystems/vcpkg.cmake" > ~/.bashrc \
    && echo "export CMAKE_PREFIX_PATH=/usr/local/vcpkg/installed/x64-linux" > ~/.bashrc

# 安装依赖
RUN vcpkg install Eigen3

# 添加 deadsnakes PPA,并安装指定 Python 版本
RUN add-apt-repository ppa:deadsnakes/ppa && apt-get update
RUN apt-get install -y \
    python${PYTHON_VERSION} \
    python${PYTHON_VERSION}-distutils \
    python${PYTHON_VERSION}-venv \
    python${PYTHON_VERSION}-dev

# 安装 pip
RUN curl -sS https://bootstrap.pypa.io/get-pip.py | python${PYTHON_VERSION}

# 创建 python3 和 pip3 的软链到指定版本
RUN update-alternatives --install /usr/bin/python3 python3 /usr/bin/python${PYTHON_VERSION} 1 && \
    ln -sf /usr/local/bin/pip /usr/bin/pip3

# 安装依赖
RUN vcpkg install pybind11
RUN pip3 install pybind11-stubgen numpy

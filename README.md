# Installation

## From PyPI

You can install the package from PyPI using pip:

```bash
pip install VVCM
```

PyPI hosts the following pre-built wheels:

- Linux: `x86_64`, `aarch64`
- Windows: `AMD64`, `ARM64`
- macOS: `x86_64`, `arm64`

> Only CPython wheels are provided. If you use PyPy, please build from source.

## From Release

In the Release of the repository, you can find pre-built binaries for various platforms:

- Linux: `x86_64`, `aarch64`
- Windows: `AMD64`, `ARM64`
- macOS: `x86_64`, `arm64`

> Only CPython wheels are provided. If you use PyPy, please build from source.

## Build From Source

### 1. Prerequisites

- Windows:
  - [MSVC](https://visualstudio.microsoft.com/)
  - [Python](https://www.python.org/) (with debugging symbols and debugging binaries)
  - [CMake](https://cmake.org/), can be installed via `install_scripts/install_cmake.windows.ps1`
  - [vcpkg](https://vcpkg.io/), can be installed via `install_scripts/install_vcpkg.windows.ps1`

- Ubuntu:
  - [GCC](https://gcc.gnu.org/) or [Clang](https://clang.llvm.org/)
  - [Python](https://www.python.org/)
  - [CMake](https://cmake.org/), can be installed via `install_scripts/install_cmake.ubuntu.bash`
  - [vcpkg](https://vcpkg.io/), can be installed via `install_scripts/install_vcpkg.ubuntu.bash`

- macOS:
  - [Xcode](https://developer.apple.com/xcode/) (with command line tools), can be installed via the App Store
  - [Python](https://www.python.org/)
  - [Homebrew](https://brew.sh/)
  - [CMake](https://cmake.org/), can be installed via `install_scripts/install_cmake.macos.zsh`
  - [vcpkg](https://vcpkg.io/), can be installed via `install_scripts/install_vcpkg.macos.zsh`

- Other Linux distributions: Please modify the installation scripts of Ubuntu.

### 2. Install dependencies

Ubuntu:

```bash
bash install_scripts/install_requirements.ubuntu.bash
```

Windows:

```powershell
& install_scripts/install_requirements.windows.ps1
```

macOS:

```zsh
zsh install_scripts/install_requirements.macos.zsh
```

### 3. Install the package

Ubuntu:

```bash
bash install_scripts/install.ubuntu.bash
```

Windows:

```powershell
& install_scripts/install.windows.ps1
```

macOS:

```zsh
zsh install_scripts/install.macos.zsh
```

# Installation

## From source

### 1. Prerequisites

- Windows:
  - [MSVC](https://visualstudio.microsoft.com/)
  - [Python](https://www.python.org/) (with debugging symbols and debugging binaries)
- Linux: [GCC](https://gcc.gnu.org/) or [Clang](https://clang.llvm.org/)
- [vcpkg](https://vcpkg.io/)
  > Can be installed via `install_scripts/install_vcpkg.bash` on Ubuntu or `install_scripts/install_vcpkg.ps1` on Windows
- [CMake](https://cmake.org/)
  > Can be installed via `install_scripts/install_cmake.bash` on Ubuntu or `install_scripts/install_cmake.ps1` on Windows

### 2. Install dependencies

Ubuntu:

```bash
bash install_scripts/install_requirements.bash
```

Windows:

```powershell
& install_scripts/install_requirements.ps1
```

### 3. Install the package

Ubuntu:

```bash
bash install_scripts/install.bash
```

Windows:

```powershell
& install_scripts/install.ps1
```

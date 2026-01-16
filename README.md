# VVCM

C++ and Python project for multi-robot transporting system with a deformable sheet, use virtual variable cables model (**VVCM**) to model the system.

## Citation

If you use the forward kinematics algorithm, please cite the following paper (bibtex):

```bibtex
@article{ma2026stable,
  title = {Stable Kinematics for Multi-Robot Collaborative Transporting System with a Deformable Sheet},
  author = {Ma, Wenyao and Hu, Jiawei and Li, Jiamao and Yi, Jingang and Xiong, Zhenhua},
  year = 2026,
  journal = {IEEE Transactions on Robotics},
  volume = {<To be assigned>},
  pages = {<To be assigned>},
  doi={10.1109/TRO.2026.3653870}
}
```

> The paper is waiting for publication, early access version can be found [here](https://ieeexplore.ieee.org/document/11353119).

Or

```bibtex
@article{hu2024forward,
  title = {Forward Kinematics of Object Transporting by a Multi-Robot System With a Deformable Sheet},
  author = {Hu, Jiawei and Liu, Wenhang and Yi, Jingang and Xiong, Zhenhua},
  year = 2024,
  journal = {IEEE Robotics and Automation Letters},
  volume = {9},
  number = {4},
  pages = {3459--3466}
}
```

If you are interested in the VVCM itself, please cite the following paper (bibtex):

```bibtex
@article{hu2022multirobot,
  title = {Multi-Robot Object Transport Motion Planning With a Deformable Sheet},
  author = {Hu, Jiawei and Liu, Wenhang and Zhang, Heng and Yi, Jingang and Xiong, Zhenhua},
  year = 2022,
  journal = {IEEE Robotics and Automation Letters},
  volume = {7},
  number = {4},
  pages = {9350--9357}
}
```

## Installation

### C++ Library

The most convenient way to use the C++ library is to copy the code and include it in your project.

> We do not provide pre-built binaries for the C++ library, and have not yet provided a vcpkg or conan package.

### Python Package

#### From PyPI

You can install the package from PyPI using pip:

```bash
pip install VVCM
```

PyPI hosts the following pre-built wheels:

- Linux: `x86_64`, `aarch64`
- Windows: `AMD64`, `ARM64`
- macOS: `x86_64`, `arm64`

> Only CPython wheels are provided. If you use PyPy, please build from source.

#### From Release

In the Release of the repository, you can find pre-built binaries for various platforms:

- Linux: `x86_64`, `aarch64`
- Windows: `AMD64`, `ARM64`
- macOS: `x86_64`, `arm64`

> Only CPython wheels are provided. If you use PyPy, please build from source.

#### Build From Source

##### 1. Prerequisites

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

##### 2. Install dependencies

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

##### 3. Install the package

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

## Usage

### C++ Library

Use stable forward kinematics algorithm to compute the object pose given robot formation and deformable sheet:

```cpp
#include <iostream>
#include <Eigen/Dense>
#include "VVCM_FK.hpp"

using namespace VVCM;

int main()
{
    int N = 4;              // number of robots
    float zr = 1000.0;      // height of the holding points (unit: mm)
    MatrixXf Rn(4, 2);      // robot formation (x, y) coordinates (unit: mm)
    Rn << 213.7, 122.7,
          804.6,  37.2,
          904.0, 550.0,
          439.3, 715.9;
    MatrixXf Vn(4, 2);      // shape of the deformable sheet (x, y) coordinates (unit: mm)
    Vn << -316.1, -421.9,
           803.4, -384.1,
           746.1,  712.8,
          -367.3,  664.2;

    std::cout << "----------------------" << std::endl;

    VVCM_FK a(N, zr, Vn);            // initialize the VVCM_FK object
    a.update_stable_solutions(Rn);   // compute the stable forward kinematics

    // M: the number of stable solutions found
    std::cout << "M: " << a.M << std::endl;

    if (a.M == 0)
    {
        std::cout << "No stable solution found." << std::endl;
        return 0;
    }

    // Po: object pose (x, y, z) coordinates for each stable solution (unit: mm)
    std::cout << "Po: " << std::endl;
    for (const auto &po : a.Po)
    {
        std::cout << po.transpose() << std::endl;
    }

    // Vo: deformable sheet vertex (x, y) coordinates for each stable solution (unit: mm)
    std::cout << "Vo: " << std::endl;
    for (const auto &vo : a.Vo)
    {
        std::cout << vo.transpose() << std::endl;
    }

    std::cout << "----------------------" << std::endl;

    return 0;
}
```

If everything is set up correctly, you should see output similar to the following:

```plain
----------------------
M: 2
Po: 
568.812 324.726 336.736
557.931 341.231 337.246
Vo: 
238.618 125.024
208.799 152.534
----------------------
```

### Python Package

Use stable forward kinematics algorithm to compute the object pose given robot formation and deformable sheet:

```python
import numpy as np
from VVCM import VVCM_FK


def test_VVCM_FK():
    N = 4  # number of robots
    zr = 1000.0  # height of the holding points (unit: mm)
    # robot formation (x, y) coordinates (unit: mm)
    Rn = np.array(
        [
            [213.7, 122.7],
            [804.6, 37.2],
            [904.0, 550.0],
            [439.3, 715.9],
        ]
    )
    # shape of the deformable sheet (x, y) coordinates (unit: mm)
    Vn = np.array(
        [
            [-316.1, -421.9],
            [803.4, -384.1],
            [746.1, 712.8],
            [-367.3, 664.2],
        ]
    )

    print("----------------------")

    a = VVCM_FK(N, zr, Vn)  # initialize the VVCM_FK object
    a.update_stable_solutions(Rn)  # compute the stable forward kinematics

    # M: the number of stable solutions found
    print(f"M: {a.M}")

    if a.M == 0:
        print("No stable solution found.")
        return

    # Po: object pose (x, y, z) coordinates for each stable solution (unit: mm)
    print("Po: ")
    for po in a.Po:
        print(po)

    # Vo: deformable sheet vertex (x, y) coordinates for each stable solution (unit: mm)
    print("Vo: ")
    for vo in a.Vo:
        print(vo)

    print("----------------------")


if __name__ == "__main__":
    test_VVCM_FK()
```

If everything is set up correctly, you should see output similar to the following:

```plain
----------------------
M: 2
Po:
[568.8123  324.72644 336.73608]
[557.9307  341.23087 337.2464 ]
Vo:
[238.6181  125.02439]
[208.79898 152.53357]
----------------------
```
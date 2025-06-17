# Python绑定

编译前，确保系统的 python3 为所需的 python3 版本。如果使用 Linux 和 Devcontainer，可以修改 `.devcontainer/devcontainer.json` 文件中的 `"PYTHON_VERSION": "3.10"` 行，将其修改为所需的 python 版本来编译。

编译后，python包位于：
- 库文件：`build/pybind11/*.so` 或 `build/pybind11/*.pyd`
- stub文件：`build/pybind11/stubs/*.pyi`

将库文件和stub文件复制到python包内即可使用。
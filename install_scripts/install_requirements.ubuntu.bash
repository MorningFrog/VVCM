#!/bin/bash

vcpkg install Eigen3
pip3 install numpy nanobind scikit-build-core[pyproject]
pip3 install matplotlib scipy
if(NOT SKBUILD)
    message(WARNING "\
  This CMake file is meant to be executed using 'scikit-build'. Running
  it directly will almost certainly not produce the desired result. If
  you are a user trying to install this package, please use the command
  below, which will install all necessary build dependencies, compile
  the package in an isolated environment, and then install it.
  =====================================================================
   $ pip install .
  =====================================================================
  If you are a software developer, and this is your own package, then
  it is usually much more efficient to install the build dependencies
  in your environment once and use the following command that avoids
  a costly creation of a new virtual environment at every compilation:
  =====================================================================
   $ pip install nanobind scikit-build-core[pyproject]
   $ pip install --no-build-isolation -ve .
  =====================================================================
  You may optionally add -Ceditable.rebuild=true to auto-rebuild when
  the package is imported. Otherwise, you need to re-run the above
  after editing C++ files.")

    # End of this CMakeLists.txt file
    return()
endif()

find_package(Eigen3 CONFIG REQUIRED)
add_definitions(${EIGEN3_DEFINITIONS})
include_directories(${EIGEN3_INCLUDE_DIRS})

# Try to import all Python components potentially needed by nanobind
find_package(Python 3.8
    REQUIRED COMPONENTS Interpreter Development.Module
    OPTIONAL_COMPONENTS Development.SABIModule)

# Import nanobind through CMake's find_package mechanism
find_package(nanobind CONFIG REQUIRED)

nanobind_add_module(
    VVCM_ext
    STABLE_ABI
    NB_STATIC
    VVCM_bindings.cpp
    ../src/VVCM_Simulation.cpp ../src/VVCM_FK.cpp ../src/VVCM_ManualSimulation.cpp
    ../include/VVCM_FK.hpp ../include/VVCM_ManualSimulation.hpp ../include/VVCM_Simulation.hpp
)

install(TARGETS VVCM_ext LIBRARY DESTINATION VVCM)
nanobind_add_stub(
    VVCM_ext_stub
    INSTALL_TIME
    MODULE VVCM_ext
    OUTPUT VVCM/VVCM_ext.pyi
    PYTHON_PATH "VVCM"
)
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

# Try to import all Python components potentially needed by nanobind
find_package(Python 3.8
    REQUIRED COMPONENTS Interpreter Development.Module
    OPTIONAL_COMPONENTS Development.SABIModule)

# Detect the installed nanobind package and import it into CMake
execute_process(
  COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir
  OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE nanobind_ROOT)
find_package(nanobind CONFIG REQUIRED)

nanobind_add_module(
    VVCM_ext
    STABLE_ABI
    NB_STATIC
    VVCM_bindings.cpp
    ../src/VVCM_Simulation.cpp ../src/VVCM_FK.cpp ../src/VVCM_ManualSimulation.cpp
    ../include/VVCM_FK.hpp ../include/VVCM_ManualSimulation.hpp ../include/VVCM_Simulation.hpp
)
target_link_libraries(VVCM_ext PRIVATE Eigen3::Eigen)

nanobind_add_stub(
    VVCM_ext_stub
    DEPENDS VVCM_ext
    PYTHON_PATH $<TARGET_FILE_DIR:VVCM_ext>
    MODULE VVCM_ext
    OUTPUT VVCM_ext.pyi
    MARKER_FILE py.typed
    VERBOSE
)

install(TARGETS VVCM_ext LIBRARY DESTINATION VVCM)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/py.typed ${CMAKE_CURRENT_BINARY_DIR}/VVCM_ext.pyi DESTINATION VVCM)

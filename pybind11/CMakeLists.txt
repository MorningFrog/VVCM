find_package(Python 3 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(vvcm_simulation VVCM_Simulation_bindings.cpp ../src/VVCM_Simulation.cpp ../src/VVCM_FK.cpp ../src/VVCM_ManualSimulation.cpp)

# 添加自定义命令,生成类型注解(pyi 文件)
add_custom_command(
    TARGET vvcm_simulation POST_BUILD
    COMMAND export PYTHONPATH=$PYTHONPATH:. && pybind11-stubgen vvcm_simulation
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating Python stubs with pybind11-stubgen for vvcm_simulation"
)
add_definitions(${EIGEN3_DEFINITIONS})
include_directories(${EIGEN3_INCLUDE_DIRS})

add_library(VVCM_FK VVCM_FK.cpp)
target_link_libraries(VVCM_FK PRIVATE Eigen3::Eigen)

add_library(VVCM_Simulation VVCM_Simulation.cpp)
target_link_libraries(VVCM_Simulation PRIVATE Eigen3::Eigen VVCM_FK)

add_library(VVCM_ManualSimulation VVCM_ManualSimulation.cpp)
target_link_libraries(VVCM_ManualSimulation PRIVATE Eigen3::Eigen VVCM_FK)
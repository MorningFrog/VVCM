find_package(Eigen3 CONFIG REQUIRED)
add_definitions(${EIGEN3_DEFINITIONS})
include_directories(${EIGEN3_INCLUDE_DIRS})

# include(GoogleTest)

# target_link_libraries(VVCM_FK_test GTest::gtest GTest::gtest_main ${EIGEN3_LIBS} VVCM_FK)

# Add test examples
add_executable(VVCM_FK_test VVCM_FK_test.cpp)
target_link_libraries(VVCM_FK_test ${EIGEN3_LIBS} VVCM_FK)

add_executable(VVCM_Simulation_test VVCM_Simulation_test.cpp)
target_link_libraries(VVCM_Simulation_test ${EIGEN3_LIBS} VVCM_Simulation)

add_executable(VVCM_ManualSimulation_test VVCM_ManualSimulation_test.cpp)
target_link_libraries(VVCM_ManualSimulation_test ${EIGEN3_LIBS} VVCM_ManualSimulation)
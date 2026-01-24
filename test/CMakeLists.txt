# include(GoogleTest)

# target_link_libraries(VVCM_FK_test GTest::gtest GTest::gtest_main Eigen3::Eigen VVCM_FK)

# Add test examples
add_executable(VVCM_FK_test VVCM_FK_test.cpp)
target_link_libraries(VVCM_FK_test Eigen3::Eigen VVCM_FK)

add_executable(VVCM_Simulation_test VVCM_Simulation_test.cpp)
target_link_libraries(VVCM_Simulation_test Eigen3::Eigen VVCM_Simulation)

add_executable(VVCM_ManualSimulation_test VVCM_ManualSimulation_test.cpp)
target_link_libraries(VVCM_ManualSimulation_test Eigen3::Eigen VVCM_ManualSimulation)
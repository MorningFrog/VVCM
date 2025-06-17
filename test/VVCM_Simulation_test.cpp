#include "VVCM_Simulation.hpp"
#include <iostream>
#include <Eigen/Dense>

using namespace VVCM;

int main()
{
    int N = 6;
    float zr = 823.0;
    MatrixXf Rn(6, 2);
    MatrixXf Vn(6, 2);
    Rn << -27.419184, -176.293854,
        398.141083, -35.190411,
        517.018127, 338.271301,
        285.155762, 609.95575,
        -175.608231, 569.463562,
        -301.437988, 194.695297;
    Vn << -131.665741, -376.508026,
        480.675873, -388.066681,
        877.700256, 217.088806,
        562.778748, 826.754089,
        -107.442101, 918.166626,
        -453.516937, 284.887146;

    std::cout << "----------------------" << std::endl;

    VVCM_Simulation a(N, zr, Vn, Rn);
    std::cout << a.global_pos << std::endl;
    std::cout << a.Rn << std::endl;
    std::cout << a.Po << std::endl;

    std::cout << "----------------------" << std::endl;

    MatrixXf v(6, 2);
    v << 5, 5,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0;
    a.set_velocity(v);
    a.step();
    std::cout << a.global_pos << std::endl;
    std::cout << a.Rn << std::endl;
    std::cout << a.Po << std::endl;

    std::cout << "----------------------" << std::endl;
}
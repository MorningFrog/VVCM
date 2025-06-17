#include "VVCM_ManualSimulation.hpp"
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

    VVCM_ManualSimulation a(N, zr, Vn);
    auto [vvcm_error, Po] = a.init(Rn);

    std::cout << Po << std::endl;

    std::tie(vvcm_error, Po) = a.get_new_stable_solution(Rn);
    std::cout << Po << std::endl;

    std::cout << "----------------------" << std::endl;
    zr /= 1000.0;
    Rn /= 1000.0;
    Vn /= 1000.0;
    VVCM_ManualSimulation b(N, zr, Vn);
    std::tie(vvcm_error, Po) = b.init(Rn);
    std::cout << Po << std::endl;

    std::tie(vvcm_error, Po) = b.get_new_stable_solution(Rn);
    std::cout << Po << std::endl;
}
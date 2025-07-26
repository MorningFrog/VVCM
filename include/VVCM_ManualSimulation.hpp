#pragma once

#include "VVCM_FK.hpp"
#include <Eigen/Dense>
#include <vector>

namespace VVCM
{

    // Simulation Engine for Multi-Robot Deformable Sheet Transport System.
    // It does not simulate the motion of the robots, but give the stable solution
    // when given the formation.
    class VVCM_ManualSimulation
    {
    public:
        VVCM_FK fk_engine;   // Forward Kinematics Engine
        Vector2f global_pos; // Global position of the formation
        MatrixXf Rn;         // Current robot formation (the true position of all robots should be Rn + global_pos)
        Vector3f Po;         // Current object position (the true position of the object should be Po + global_pos)
        IntVector It;        // The taut cable set
        int solution_idx;    // Index of the solution in the fk_engine

        VVCM_ManualSimulation(int N, float zr, const MatrixXf &Vn) : fk_engine(N, zr, Vn) {};

        /**
         * @brief init the engine, all the unit of length is mm or s.
         *
         * @param Rn_initial current robot formation
         * @param Po_initial current Po (unimportant, it affets the solution choosen)
         * @return Error info, Po
         */
        std::tuple<VVCM_FK_Error, Vector3f> init(const MatrixXf &Rn_initial, const Vector3f &Po_initial = Vector3f(0.0, 0.0, 0.0));

        /**
         * @brief Get new stable solution with changed formation.
         */
        std::tuple<VVCM_FK_Error, Vector3f> get_new_stable_solution(const MatrixXf &Rn);

    private:
        void get_closest_solution(Vector3f Po_ref);
    };

} // namespace VVCM
#pragma once

#include "VVCM_FK.hpp"
#include <Eigen/Dense>
#include <vector>

namespace VVCM
{

    // Simulation Engine for Multi-Robot Deformable Sheet Transport System
    class VVCM_Simulation
    {
    public:
        VVCM_FK fk_engine;   // Forward Kinematics Engine
        Vector2f global_pos; // Global position of the formation
        MatrixXf Rn;         // Current robot formation (the true position of all robots should be Rn + global_pos)
        Vector3f Po;         // Current object position (the true position of the object should be Po + global_pos)
        IntVector It;        // The taut cable set
        int solution_idx;    // Index of the solution in the fk_engine
        float dt;            // Time step for the simulation
        MatrixXf Rn_vel;     // Velocity of the robots (N x 2)

        /**
         * @brief init the engine, all the unit of length is mm or s.
         *
         * @param N robot number
         * @param zr the height of holding point
         * @param Vn sheet shape
         * @param Rn_initial current robot formation
         * @param Po_initial current Po (unimportant, it affets the solution choosen)
         * @param dt time step for the simulation
         */
        VVCM_Simulation(int N, float zr, const MatrixXf &Vn, const MatrixXf &Rn_initial, const Vector3f &Po_initial = Vector3f(0.0, 0.0, 0.0), float dt = 1.0 / 30.0);

        /**
         * @brief Set velocity for the robot formation
         *
         * @param Rn_velocity N x 2 velocity vector
         */
        void set_velocity(MatrixXf &Rn_velocity);

        void step();

        /**
         * @brief Get the Absolute Rn object
         *
         * @return true position of all robots
         */
        Eigen::MatrixX2f get_absolute_Rn();

    private:
        void get_closest_solution(Vector3f Po_ref);
    };

} // namespace VVCM
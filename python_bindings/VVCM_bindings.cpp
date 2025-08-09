#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/eigen/dense.h>
#include "VVCM_FK.hpp"
#include "VVCM_Simulation.hpp"
#include "VVCM_ManualSimulation.hpp"

namespace nb = nanobind;
using namespace nb::literals;

namespace VVCM
{
    NB_MODULE(VVCM_ext, m)
    {
        m.doc() = "VVCM Simulation module for simulating multi-robot deformable sheet transport system";

        // Export VVCM_FK_Error class
        nb::enum_<VVCM_FK_Error>(m, "VVCM_FK_Error", "Error Type for VVCM_FK")
            .value("NoError", VVCM_FK_Error::NoError, "No Error")
            .value("NoSolution", VVCM_FK_Error::NoSolution, "No Solution")
            .value("NoStableSolution", VVCM_FK_Error::NoStableSolution, "No Stable Solution")
            .value("InFeasibleFormation", VVCM_FK_Error::InFeasibleFormation, "Rn is not inside Vn");

        // Export VVCM_FK class
        nb::class_<VVCM_FK>(m, "VVCM_FK", "Get Stable Solutions of Forward Kinematics for Multi-Robot Deformable Sheet Transport System")
            .def(nb::init<int, float, const Eigen::MatrixXf &>(),
                 "N"_a, "zr"_a, "Vn"_a)
            .def("update_stable_solutions", &VVCM_FK::update_stable_solutions,
                 "Rn"_a)
            .def_ro("N", &VVCM_FK::N, "Number of robots")
            .def_ro("zr", &VVCM_FK::zr, "Height of holding point")
            .def_ro("Vn", &VVCM_FK::Vn, "Sheet shape")
            .def_ro("M", &VVCM_FK::M, "Number of stable solutions")
            .def_ro("Rn", &VVCM_FK::Rn, "Current robot formation")
            .def_ro("Po", &VVCM_FK::Po, "Object positions in world frame in all stable solutions")
            .def_ro("Vo", &VVCM_FK::Vo, "Object positions in sheet frame in all stable solutions")
            .def_ro("It", &VVCM_FK::It, "Taut cable set in all stable solutions")
            .def_ro("Tn", &VVCM_FK::Tn, "Number of taut cables in each solution")
            .def_ro("ITn", &VVCM_FK::ITn, "Number of non-taut cables in each solution")
            .def_ro("stable_idxes", &VVCM_FK::stable_idxes, "Indexes of stable solutions")
            .def_ro("M_all", &VVCM_FK::M_all, "Number of all solutions (regardless of stability)")
            .def_ro("Po_all", &VVCM_FK::Po_all, "Object positions in world frame in all solutions (regardless of stability)")
            .def_ro("Vo_all", &VVCM_FK::Vo_all, "Object positions in sheet frame in all solutions (regardless of stability)")
            .def_ro("It_all", &VVCM_FK::It_all, "Taut cable set in all solutions (regardless of stability)")
            .def_ro("Tn_all", &VVCM_FK::Tn_all, "Number of taut cables in each solution (regardless of stability)")
            .def_ro("ITn_all", &VVCM_FK::ITn_all, "Number of non-taut cables in each solution (regardless of stability)");

        // Export VVCM_Simulation class
        nb::class_<VVCM_Simulation>(m, "VVCM_Simulation", "Simulate the VVCM system")
            .def(nb::init<int, float, const Eigen::MatrixXf &, const Eigen::MatrixXf &, const Eigen::Vector3f &, float>(),
                 "N"_a, "zr"_a, "Vn"_a, "Rn_initial"_a, "Po_initial"_a = Eigen::Vector3f(0.0, 0.0, 0.0), "dt"_a = 1.0 / 30.0,
                 R"start(init the engine, all the unit of length is mm or s.

Args:
    N: robot number
    zr: the height of holding point
    Vn: sheet shape
    Rn_initial: current robot formation
    Po_initial: current Po (unimportant, it affets the solution choosen)
    dt: time step for the simulation)start")
            .def("set_velocity", &VVCM_Simulation::set_velocity, "Rn_velocity"_a,
                 R"start(Set velocity for the robot formation

Args:
    Rn_velocity: N x 2 velocity vector)start")
            .def("step", &VVCM_Simulation::step, "Simulation step")
            .def("get_absolute_Rn", &VVCM_Simulation::get_absolute_Rn,
                 R"start(Get the Absolute Rn object
Returns:
    true position of all robots)start")
            .def_ro("fk_engine", &VVCM_Simulation::fk_engine, "Forward Kinematics Engine")
            .def_ro("global_pos", &VVCM_Simulation::global_pos, "Global position of the formation")
            .def_ro("Rn", &VVCM_Simulation::Rn, "Current robot formation (the true position of all robots should be Rn + global_pos)")
            .def_ro("Po", &VVCM_Simulation::Po, "Current object position (the true position of the object should be Po + global_pos)")
            .def_ro("It", &VVCM_Simulation::It, "The taut cable set")
            .def_ro("solution_idx", &VVCM_Simulation::solution_idx, "Index of the solution in the fk_engine")
            .def_ro("dt", &VVCM_Simulation::dt, "Time step for the simulation")
            .def_ro("Rn_vel", &VVCM_Simulation::Rn_vel, "Velocity of the robots (N x 2)");

        // Export VVCM_ManualSimulation class
        nb::class_<VVCM_ManualSimulation>(m, "VVCM_ManualSimulation", R"start(Simulation Engine for Multi-Robot Deformable Sheet Transport System.
It does not simulate the motion of the robots, but give the stable solution
when given the formation.)start")
            .def(nb::init<int, float, const MatrixXf &>(), "N"_a, "zr"_a, "Vn"_a)
            .def("init", &VVCM_ManualSimulation::init, "Rn_initial"_a, "Po_initial"_a = Vector3f(0.0, 0.0, 0.0),
                 R"start(init the engine, all the unit of length is mm or s.

Args:
    Rn_initial: current robot formation
    Po_initial: current Po (unimportant, it affets the solution choosen)

Returns:
    Po)start")
            .def("get_new_stable_solution", &VVCM_ManualSimulation::get_new_stable_solution, "Rn"_a,
                 R"start(Get new stable solution with changed formation.

Args:
    Rn: current robot formation

Returns:
    Error info
    Po)start")
            .def_ro("fk_engine", &VVCM_ManualSimulation::fk_engine, "Forward Kinematics Engine")
            .def_ro("global_pos", &VVCM_ManualSimulation::global_pos, "Global position of the formation")
            .def_ro("Rn", &VVCM_ManualSimulation::Rn, "Current robot formation (the true position of all robots should be Rn + global_pos)")
            .def_ro("Po", &VVCM_ManualSimulation::Po, "Current object position (the true position of the object should be Po + global_pos)")
            .def_ro("It", &VVCM_ManualSimulation::It, "The taut cable set")
            .def_ro("solution_idx", &VVCM_ManualSimulation::solution_idx, "Index of the solution in the fk_engine");
    }
}
#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/stl.h>
#include "VVCM_FK.hpp"
#include "VVCM_Simulation.hpp"
#include "VVCM_ManualSimulation.hpp"

namespace py = pybind11;

namespace VVCM
{
    PYBIND11_MODULE(vvcm_simulation, m)
    {
        m.doc() = "VVCM Simulation module for simulating multi-robot deformable sheet transport system";

        // 导出 VVCM_FK_Error 类
        py::enum_<VVCM_FK_Error>(m, "VVCM_FK_Error", "Error Type for VVCM_FK")
            .value("NoError", VVCM_FK_Error::NoError, "No Error")
            .value("NoSolution", VVCM_FK_Error::NoSolution, "No Solution")
            .value("NoStableSolution", VVCM_FK_Error::NoStableSolution, "No Stable Solution")
            .value("InFeasibleFormation", VVCM_FK_Error::InFeasibleFormation, "Rn is not inside Vn");

        // 导出 VVCM_FK 类
        py::class_<VVCM_FK>(m, "VVCM_FK", "Get Stable Solutions of Forward Kinematics for Multi-Robot Deformable Sheet Transport System")
            .def(py::init<int, float, const MatrixXf &>(), py::arg("N"), py::arg("zr"), py::arg("Vn"))
            .def("update_stable_solutions", &VVCM_FK::update_stable_solutions, py::arg("Rn"))
            .def_readonly("N", &VVCM_FK::N, "Number of robots")
            .def_readonly("zr", &VVCM_FK::zr, "Height of holding point")
            .def_readonly("Vn", &VVCM_FK::Vn, "Sheet shape")
            .def_readonly("M", &VVCM_FK::M, "Number of stable solutions")
            .def_readonly("Rn", &VVCM_FK::Rn, "Current robot formation")
            .def_readonly("Po", &VVCM_FK::Po, "Object positions in world frame in all stable solutions")
            .def_readonly("Vo", &VVCM_FK::Vo, "Object positions in sheet frame in all stable solutions")
            .def_readonly("It", &VVCM_FK::It, "Taut cable set in all stable solutions")
            .def_readonly("Tn", &VVCM_FK::Tn, "Number of taut cables in each solution")
            .def_readonly("ITn", &VVCM_FK::ITn, "Number of non-taut cables in each solution");

        // 导出 VVCM_Simulation 类
        py::class_<VVCM_Simulation>(m, "VVCM_Simulation", "Simulation Engine for Multi-Robot Deformable Sheet Transport System")
            .def(py::init<int, float, const MatrixXf &, const MatrixXf &, const Vector3f &, float>(),
                 py::arg("N"), py::arg("zr"), py::arg("Vn"), py::arg("Rn_initial"), py::arg("Po_initial") = Vector3f(0.0, 0.0, 0.0), py::arg("dt") = 1.0 / 30.0,
                 R"start(init the engine, all the unit of length is mm or s.

Args:
    N: robot number
    zr: the height of holding point
    Vn: sheet shape
    Rn_initial: current robot formation
    Po_initial: current Po (unimportant, it affets the solution choosen)
    dt: time step for the simulation)start")
            .def("set_velocity", &VVCM_Simulation::set_velocity, py::arg("Rn_velocity"),
                 R"start(Set velocity for the robot formation

Args:
    Rn_velocity: N x 2 velocity vector)start")
            .def("step", &VVCM_Simulation::step, "Simulation step")
            .def("get_absolute_Rn", &VVCM_Simulation::get_absolute_Rn,
                 R"start(Get the Absolute Rn object
Returns:
    true position of all robots)start")
            .def_readonly("fk_engine", &VVCM_Simulation::fk_engine, "Forward Kinematics Engine")
            .def_readonly("global_pos", &VVCM_Simulation::global_pos, "Global position of the formation")
            .def_readonly("Rn", &VVCM_Simulation::Rn, "Current robot formation (the true position of all robots should be Rn + global_pos)")
            .def_readonly("Po", &VVCM_Simulation::Po, "Current object position (the true position of the object should be Po + global_pos)")
            .def_readonly("It", &VVCM_Simulation::It, "The taut cable set")
            .def_readonly("solution_idx", &VVCM_Simulation::solution_idx, "Index of the solution in the fk_engine")
            .def_readonly("dt", &VVCM_Simulation::dt, "Time step for the simulation")
            .def_readonly("Rn_vel", &VVCM_Simulation::Rn_vel, "Velocity of the robots (N x 2)");

        // 导出 VVCM_ManualSimulation 类
        py::class_<VVCM_ManualSimulation>(m, "VVCM_ManualSimulation", R"start(Simulation Engine for Multi-Robot Deformable Sheet Transport System.
It does not simulate the motion of the robots, but give the stable solution
when given the formation.)start")
            .def(py::init<int, float, const MatrixXf &>(), py::arg("N"), py::arg("zr"), py::arg("Vn"))
            .def("init", &VVCM_ManualSimulation::init, py::arg("Rn_initial"), py::arg("Po_initial") = Vector3f(0.0, 0.0, 0.0),
                 R"start(init the engine, all the unit of length is mm or s.

Args:
    Rn_initial: current robot formation
    Po_initial: current Po (unimportant, it affets the solution choosen)

Returns:
    Po)start")
            .def("get_new_stable_solution", &VVCM_ManualSimulation::get_new_stable_solution, py::arg("Rn"),
                 R"start(Get new stable solution with changed formation.

Args:
    Rn: current robot formation

Returns:
    Error info
    Po)start")
            .def_readonly("fk_engine", &VVCM_ManualSimulation::fk_engine, "Forward Kinematics Engine")
            .def_readonly("global_pos", &VVCM_ManualSimulation::global_pos, "Global position of the formation")
            .def_readonly("Rn", &VVCM_ManualSimulation::Rn, "Current robot formation (the true position of all robots should be Rn + global_pos)")
            .def_readonly("Po", &VVCM_ManualSimulation::Po, "Current object position (the true position of the object should be Po + global_pos)")
            .def_readonly("It", &VVCM_ManualSimulation::It, "The taut cable set")
            .def_readonly("solution_idx", &VVCM_ManualSimulation::solution_idx, "Index of the solution in the fk_engine");
    }
}
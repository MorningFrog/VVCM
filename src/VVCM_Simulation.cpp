#include "VVCM_Simulation.hpp"

namespace VVCM
{

    VVCM_Simulation::VVCM_Simulation(int N, float zr, const MatrixXf &Vn, const MatrixXf &Rn_initial, const Vector3f &Po_initial, float dt)
        : fk_engine(N, zr, Vn), global_pos(Rn_initial.row(0)), dt(dt), Rn_vel(MatrixXf::Zero(N, 2))
    {
        this->Rn = Rn_initial.rowwise() - global_pos.transpose();
        this->fk_engine.update_stable_solutions(Rn);

        if (this->fk_engine.M == 0)
        {
            throw std::runtime_error("No solution for the formation!");
        }
        else if (this->fk_engine.M == 1)
        {
            this->Po = this->fk_engine.Po[0];
            this->It = this->fk_engine.It[0];
            this->solution_idx = 0;
        }
        else
        {
            Vector3f adjusted_Po_initial = Po_initial;
            adjusted_Po_initial.head<2>() -= this->global_pos;
            get_closest_solution(adjusted_Po_initial);
        }
    }

    void VVCM_Simulation::set_velocity(MatrixXf &Rn_velocity)
    {
        this->Rn_vel = Rn_velocity;
    }

    void VVCM_Simulation::step()
    {
        if ((Rn_vel.array() == 0).all())
        {
            return;
        }

        Vector2f d_global_pos = this->Rn_vel.row(0) * dt;
        this->global_pos += d_global_pos;
        this->Rn.bottomRows(this->Rn.rows() - 1) += (this->Rn_vel.bottomRows(this->Rn_vel.rows() - 1) * this->dt).rowwise() - d_global_pos.transpose();

        this->fk_engine.update_stable_solutions(this->Rn);

        if (this->fk_engine.M == 0)
        {
            throw std::runtime_error("No solution for the formation!");
        }

        get_closest_solution(Po);
    }

    Eigen::MatrixX2f VVCM_Simulation::get_absolute_Rn()
    {
        return Rn.rowwise() + global_pos.transpose();
    }

    void VVCM_Simulation::get_closest_solution(Vector3f Po_ref)
    {
        std::vector<float> distances(fk_engine.Po.size());
        std::transform(fk_engine.Po.begin(), fk_engine.Po.end(), distances.begin(),
                       [&](const Vector3f &row)
                       { return (row - Po_ref).norm(); });
        solution_idx = std::distance(distances.begin(), std::min_element(distances.begin(), distances.end()));
        Po = fk_engine.Po[solution_idx];
        It = fk_engine.It[solution_idx];
    }

} // namespace VVCM
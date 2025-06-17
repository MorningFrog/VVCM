#include "VVCM_ManualSimulation.hpp"

namespace VVCM
{

    std::tuple<VVCM_FK_Error, Vector3f> VVCM_ManualSimulation::init(const MatrixXf &Rn_initial, const Vector3f &Po_initial)
    {
        global_pos(0) = Rn_initial.col(0).mean();
        global_pos(1) = Rn_initial.col(1).mean();

        Rn = Rn_initial.rowwise() - global_pos.transpose();
        VVCM_FK_Error vvcm_error = fk_engine.update_stable_solutions(Rn);

        if (fk_engine.M == 0)
        {
            return {vvcm_error, Po_initial};
        }
        else if (fk_engine.M == 1)
        {
            Po = fk_engine.Po[0];
            It = fk_engine.It[0];
            solution_idx = 0;
        }
        else
        {
            Vector3f adjusted_Po_initial = Po_initial;
            adjusted_Po_initial.head<2>() -= global_pos;
            get_closest_solution(adjusted_Po_initial);
        }

        Vector3f Po_global = Po;
        Po_global.head<2>() += global_pos;

        return {vvcm_error, Po_global};
    }

    std::tuple<VVCM_FK_Error, Vector3f> VVCM_ManualSimulation::get_new_stable_solution(const MatrixXf &Rn)
    {
        this->global_pos(0) = Rn.col(0).mean();
        this->global_pos(1) = Rn.col(1).mean();
        this->Rn = Rn.rowwise() - global_pos.transpose();
        VVCM_FK_Error vvcm_error = fk_engine.update_stable_solutions(this->Rn);

        if (fk_engine.M == 0)
        {
            return {vvcm_error, Vector3f::Zero()};
        }

        get_closest_solution(Po);

        Vector3f Po_global = Po;
        Po_global.head<2>() += global_pos;

        return {vvcm_error, Po_global};
    }

    void VVCM_ManualSimulation::get_closest_solution(Vector3f Po_ref)
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
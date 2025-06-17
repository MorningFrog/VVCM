#pragma once

#include <vector>
#include <array>
#include <tuple>
#include <Eigen/Dense>

namespace VVCM
{
    using Vector3f = Eigen::Vector3f;
    using Vector2f = Eigen::Vector2f;
    using VectorXf = Eigen::VectorXf;
    using MatrixXf = Eigen::MatrixXf;
    using IntVector = std::vector<int>;
    using Vector3fVector = std::vector<Vector3f>;
    using Vector2fVector = std::vector<Vector2f>;
    using IntVectorVector = std::vector<IntVector>;
    using VectorXfVector = std::vector<VectorXf>;
    using MatrixXfVector = std::vector<MatrixXf>;

    enum class VVCM_FK_Error
    {
        NoError,            // 无错误
        NoSolution,         // 无解
        NoStableSolution,   // 无稳定解
        InFeasibleFormation // Rn is not inside Vn
    };

    // Get Stable Solutions of Forward Kinematics for
    // Multi-Robot Deformable Sheet Transport System
    class VVCM_FK
    {
    public:
        int N;
        float zr;
        MatrixXf Vn;

        int M;
        MatrixXf Rn;
        Vector3fVector Po;
        Vector2fVector Vo;
        IntVectorVector It;
        IntVector Tn;
        IntVector ITn;

        VVCM_FK(int N, float zr, const MatrixXf &Vn);

        VVCM_FK_Error update_stable_solutions(const MatrixXf &Rn);

    private:
        /**
         * @brief This function is to find all stable Forward Kinematics solutions.
         *
         * @return error_info, M, Po, Vo, It, Tn, ITn, lambda, Omega_idx, Omega
         */
        std::tuple<VVCM_FK_Error, int, Vector3fVector, Vector2fVector, IntVectorVector, IntVector, IntVector, VectorXfVector, IntVector, MatrixXfVector>
        get_solutions_inside_polygon();

        /**
         * @brief Determine whether Rn is inside Vn
         */
        bool formation_feasible();

        /**
         * @brief This function is calculate the Forward Kinematics
         *        when taut cable group is known.
         *
         * @param taut_vec taut cable set
         * @return is_possible, Po, Vo, lambda, n_indep, Omega
         */
        std::tuple<bool, Vector3f, Vector2f, VectorXf, int, MatrixXf>
        vvcm_CQP(const IntVector &taut_vec);

        bool in_polygon(float x, float y, const std::vector<float> &xv, const std::vector<float> &yv);
        VectorXf get_local_minimalism(int M, const IntVector &Tn, const IntVector &ITn, const VectorXfVector &lambda, const IntVector &Omega_idx, const MatrixXfVector &Omega);
    };

} // namespace VVCM
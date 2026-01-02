#include "VVCM_FK.hpp"
#include <Eigen/Core>
#include <Eigen/LU>
#include <unsupported/Eigen/MatrixFunctions>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <iterator>
#include <tuple>
#include <iostream>

namespace VVCM
{

    VVCM_FK::VVCM_FK(int N, float zr, const MatrixXf &Vn)
        : N(N), zr(zr), Vn(Vn),
          M(0), Rn(), Po(), Vo(), It(), Tn(), ITn(), stable_idxes(),
          M_all(0), Po_all(), Vo_all(), It_all(), Tn_all(), ITn_all() {}

    VVCM_FK_Error VVCM_FK::update_stable_solutions(const MatrixXf &Rn)
    {
        // Clear previous results
        this->M = 0;
        this->Po.clear();
        this->Vo.clear();
        this->It.clear();
        this->Tn.clear();
        this->ITn.clear();
        this->stable_idxes.clear();

        // Update robot formation and all solutions
        this->Rn = Rn;
        auto [vvcm_error, M_all, Po_all, Vo_all, It_all, Tn_all, ITn_all, lambda, Omega_idx, Omega] = get_solutions_inside_polygon();

        this->M_all = M_all;
        this->Po_all = Po_all;
        this->Vo_all = Vo_all;
        this->It_all = It_all;
        this->Tn_all = Tn_all;
        this->ITn_all = ITn_all;

        if (M_all == 0)
        {
            return vvcm_error;
        }

        auto local_minimalism = get_local_minimalism(M_all, Tn_all, ITn_all, lambda, Omega_idx, Omega);
        for (int i = 0; i < local_minimalism.size(); ++i)
        {
            if (local_minimalism(i) != 0)
            {
                this->stable_idxes.push_back(i);
            }
        }
        this->M = this->stable_idxes.size();
        for (auto i : this->stable_idxes)
        {
            this->Po.push_back(Po_all[i]);
            this->Vo.push_back(Vo_all[i]);
            this->It.push_back(It_all[i]);
            this->Tn.push_back(Tn_all[i]);
            this->ITn.push_back(ITn_all[i]);
        }

        if (this->M == 0)
        {
            return VVCM_FK_Error::NoStableSolution;
        }
        else
        {
            return VVCM_FK_Error::NoError;
        }
    }

    bool next_combination(IntVector &comb, int N, int K)
    {
        for (int i = K - 1; i >= 0; --i)
        {
            if (comb[i] < N - K + i + 1)
            {
                ++comb[i];
                for (int j = i + 1; j < K; ++j)
                {
                    comb[j] = comb[j - 1] + 1;
                }
                return true;
            }
        }
        return false;
    }

    std::tuple<VVCM_FK_Error, int, Vector3fVector, Vector2fVector, IntVectorVector, IntVector, IntVector, VectorXfVector, IntVector, MatrixXfVector>
    VVCM_FK::get_solutions_inside_polygon()
    {
        int M = 0;
        Vector3fVector Po;
        Vector2fVector Vo;
        IntVectorVector It;
        IntVector Tn;
        IntVector ITn;
        VectorXfVector lambda;
        IntVector Omega_idx;
        MatrixXfVector Omega;

        int Omega_idx_current = 0;

        if (!formation_feasible())
        {
            return {VVCM_FK_Error::InFeasibleFormation, M, Po, Vo, It, Tn, ITn, lambda, Omega_idx, Omega};
        }

        // K = 3:N or 3:5 almost have no effect on the result
        for (int K = 3; K <= 5 && K <= N; ++K)
        {
            IntVector comb(K);
            std::iota(comb.begin(), comb.end(), 1);

            do
            {
                IntVector taut_vec(comb.begin(), comb.end());

                auto [is_possible, Po_temp, Vo_temp, lambda_temp, n_indep, Omega_temp] = vvcm_CQP(taut_vec);
                if (!is_possible)
                    continue;

                M++;
                Po.push_back(Po_temp);
                Vo.push_back(Vo_temp);
                It.push_back(taut_vec);
                Tn.push_back(K);
                ITn.push_back(n_indep);
                lambda.push_back(lambda_temp);
                if (K == n_indep)
                {
                    Omega_idx.push_back(-1);
                }
                else
                {
                    Omega_idx.push_back(Omega_idx_current);
                    Omega_idx_current++;
                    Omega.push_back(Omega_temp);
                }
            } while (next_combination(comb, N, K));
        }

        if (M == 0)
        {
            return {VVCM_FK_Error::NoSolution, M, Po, Vo, It, Tn, ITn, lambda, Omega_idx, Omega};
        }
        else
        {
            return {VVCM_FK_Error::NoError, M, Po, Vo, It, Tn, ITn, lambda, Omega_idx, Omega};
        }
    }

    bool VVCM_FK::formation_feasible()
    {
        int num_rows_Vn = Vn.rows();
        int num_rows_Rn = Rn.rows();

        MatrixXf distV(num_rows_Vn, num_rows_Vn);
        MatrixXf distR(num_rows_Rn, num_rows_Rn);

        // Calculate distance matrix of Vn
        for (int i = 0; i < num_rows_Vn; ++i)
        {
            for (int j = 0; j < num_rows_Vn; ++j)
            {
                distV(i, j) = (Vn.row(i) - Vn.row(j)).norm();
            }
        }

        // Calculate distance matrix of Rn
        for (int i = 0; i < num_rows_Rn; ++i)
        {
            for (int j = 0; j < num_rows_Rn; ++j)
            {
                distR(i, j) = (Rn.row(i) - Rn.row(j)).norm();
            }
        }

        // Check if all distances in Rn are less than or equal to the corresponding distances in Vn
        return (distR.array() <= distV.array()).all();
    }

    std::tuple<bool, Vector3f, Vector2f, VectorXf, int, MatrixXf>
    VVCM_FK::vvcm_CQP(const IntVector &It)
    {
        IntVector cable_all(N);
        std::iota(cable_all.begin(), cable_all.end(), 1);
        IntVector Is;
        std::set_difference(cable_all.begin(), cable_all.end(), It.begin(), It.end(), std::inserter(Is, Is.begin()));

        int n_slack = Is.size();
        int K = It.size();

        VectorXf xv = Vn.col(0);
        VectorXf yv = Vn.col(1);
        VectorXf x = Rn.col(0);
        VectorXf y = Rn.col(1);

        Eigen::VectorXi id2toN(K - 1 + n_slack);
        for (int i = 0; i < K - 1; ++i)
            id2toN[i] = It[i + 1];
        for (int i = 0; i < n_slack; ++i)
            id2toN[K - 1 + i] = Is[i];
        int id1 = It[0];

        MatrixXf A(id2toN.size(), 4);
        A << x(id1 - 1) - x(id2toN.array() - 1).array(),
            y(id1 - 1) - y(id2toN.array() - 1).array(),
            xv(id2toN.array() - 1).array() - xv(id1 - 1),
            yv(id2toN.array() - 1).array() - yv(id1 - 1);

        VectorXf b = 0.5 * ((VectorXf::Constant(id2toN.size(), x(id1 - 1)).array().square() +
                             VectorXf::Constant(id2toN.size(), y(id1 - 1)).array().square() -
                             VectorXf::Constant(id2toN.size(), xv(id1 - 1)).array().square() -
                             VectorXf::Constant(id2toN.size(), yv(id1 - 1)).array().square() +
                             xv(id2toN.array() - 1).array().square() +
                             yv(id2toN.array() - 1).array().square() -
                             x(id2toN.array() - 1).array().square() -
                             y(id2toN.array() - 1).array().square()));

        MatrixXf A1 = A.topRows(K - 1);
        VectorXf b1 = b.head(K - 1);

        MatrixXf A1_bar(A1.rows(), A1.cols() + 1);
        A1_bar << A1, b1;

        bool rank_OK = (A1.fullPivLu().rank() == A1_bar.fullPivLu().rank());

        bool is_possible = false;
        Vector3f Po;
        Vector2f Vo;
        VectorXf lambda;
        int n_indep;
        MatrixXf Omega;

        if (rank_OK)
        {
            MatrixXf L = A1_bar.householderQr().matrixQR().triangularView<Eigen::Upper>();
            MatrixXf C = MatrixXf::Identity(L.rows(), L.rows());
            for (int i = 0; i < L.rows(); ++i)
            {
                if (L(i, i) == 0)
                    continue;
                C.row(i) /= L(i, i);
            }

            MatrixXf A11_bar = A1_bar(Eigen::indexing::all, Eigen::indexing::seqN(0, A1_bar.cols()));
            MatrixXf A11 = A11_bar.leftCols(A1.cols());
            VectorXf b11 = A11_bar.rightCols(1);

            n_indep = C.rows() + 1;

            Omega = MatrixXf::Zero(n_indep, K);
            Omega(0, 0) = 1;
            for (int i = 1; i < K; ++i)
            {
                Omega(0, i) = 1 - C.col(i - 1).head(n_indep - 1).sum();
                Omega.col(i).tail(n_indep - 1) = C.col(i - 1).head(n_indep - 1);
            }

            MatrixXf Q = (MatrixXf(4, 4) << 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, -2, 0, 0, 0, 0, -2).finished();
            VectorXf c(4);
            c << -2 * x(id1 - 1), -2 * y(id1 - 1), 2 * xv(id1 - 1), 2 * yv(id1 - 1);
            float f0 = x(id1 - 1) * x(id1 - 1) + y(id1 - 1) * y(id1 - 1) - xv(id1 - 1) * xv(id1 - 1) - yv(id1 - 1) * yv(id1 - 1);

            MatrixXf Lagrange_matrix = MatrixXf::Zero(8, 8);
            Lagrange_matrix.topLeftCorner(4, 4) = Q;
            Lagrange_matrix.topRightCorner(4, A11.rows()) = A11.transpose();
            Lagrange_matrix.bottomLeftCorner(A11.rows(), 4) = A11;

            VectorXf rhs = VectorXf::Zero(8);
            rhs.head(4) = -c;
            rhs.tail(A11.rows()) = b11;

            VectorXf solution = Lagrange_matrix.lu().solve(rhs);
            VectorXf x_bar = solution.head(4);
            lambda = solution.tail(A11.rows());

            VectorXf lambda_new(lambda.size() + 1);
            lambda_new(0) = (2 - lambda.sum()) / 2.0;
            lambda_new.tail(lambda.size()) = lambda / 2.0;
            lambda = lambda_new;

            float x_o = x_bar(0);
            float y_o = x_bar(1);
            float x_vo = x_bar(2);
            float y_vo = x_bar(3);

            float term1 = 0.5 * (x_bar.transpose() * Q * x_bar).value();
            float term2 = (c.transpose() * x_bar).value();
            float tmp = -(term1 + term2 + f0);

            if (tmp < 0)
            {
                is_possible = false;
            }
            else
            {
                float zo = zr - std::sqrt(tmp);

                std::vector<float> x_taut(It.size()), y_taut(It.size());
                for (size_t i = 0; i < It.size(); ++i)
                {
                    x_taut[i] = x(It[i] - 1);
                    y_taut[i] = y(It[i] - 1);
                }
                bool polygon_OK = in_polygon(x_o, y_o, x_taut, y_taut);

                bool rest_OK = (n_slack == 0) || (b.tail(n_slack) - A.bottomRows(n_slack) * x_bar).array().minCoeff() > 1e-8;

                is_possible = rank_OK && rest_OK && polygon_OK;

                if (is_possible)
                {
                    Po << x_o, y_o, zo;
                    Vo << x_vo, y_vo;
                }
            }
        }

        return {is_possible, Po, Vo, lambda, n_indep, Omega};
    }

    bool VVCM_FK::in_polygon(float x, float y, const std::vector<float> &xv, const std::vector<float> &yv)
    {
        bool inside = false;
        for (size_t i = 0, j = xv.size() - 1; i < xv.size(); j = i++)
        {
            if (((yv[i] > y) != (yv[j] > y)) && (x < (xv[j] - xv[i]) * (y - yv[i]) / (yv[j] - yv[i]) + xv[i]))
            {
                inside = !inside;
            }
        }
        return inside;
    }

    VectorXf VVCM_FK::get_local_minimalism(int M, const IntVector &Tn, const IntVector &ITn, const VectorXfVector &lambda, const IntVector &Omega_idx, const MatrixXfVector &Omega)
    {
        VectorXf local_minimalism = VectorXf::Zero(M);
        for (int i = 0; i < M; ++i)
        {
            const auto &lambda0 = lambda[i];

            if ((lambda0.array() >= -1e-8).all())
            {
                local_minimalism(i) = 1;
                continue;
            }

            if (Omega_idx[i] == -1)
            {
                continue;
            }
            int idx = Omega_idx[i];
            int k = ITn[i];
            int K = Tn[i];

            std::cout << k << " " << K << " " << idx << std::endl;

            IntVectorVector new_combinations;
            IntVector combination(k);
            std::iota(combination.begin(), combination.end(), 0);
            do
            {
                new_combinations.push_back(combination);
            } while (std::next_permutation(combination.begin(), combination.end()));

            bool flag = false;
            for (const auto &comb : new_combinations)
            {
                MatrixXf C(k, comb.size());
                for (size_t j = 0; j < comb.size(); ++j)
                {
                    C.col(j) = Omega[idx].col(comb[j]);
                }

                if (C.fullPivLu().rank() < k)
                {
                    continue;
                }

                VectorXf lambda1 = C.lu().solve(lambda0);
                if ((lambda1.array() >= -1e-8).all())
                {
                    local_minimalism(i) = 1;
                    flag = true;
                    break;
                }
            }
        }
        return local_minimalism;
    }
} // namespace VVCM
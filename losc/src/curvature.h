#ifndef _LOSC_CURVATURE_H_
#define _LOSC_CURVATURE_H_

#include <memory>
#include <vector>
#include <matrix/matrix.h>

namespace losc {

using std::vector;
using matrix::Matrix;
using SharedMatrix = std::shared_ptr<Matrix>;
using SharedDoubleVector = std::shared_ptr<vector<double>>;

enum DFAType {
    LDA,
    GGA,
    B3LYP,
};

class CurvatureBase {
    protected:
    DFAType dfa_type_;

    size_t nlo_;
    size_t nbasis_;
    size_t nfitbasis_;
    size_t npts_;

    double para_alpha_ = 0.0;
    double para_beta_ = 0.0;

    /**
     * LO coefficient matrix under AO.
     * Dimension: nlo x nbasis.
     * It can be provided by `losc::LocalizerBase::compute()` function.
     */
    SharedMatrix C_lo_;

    /**
     * three-body integral of <p|mn>.
     * m, n is the AO basis index, and p is the fitbasis index.
     * Dimension: [nfitbasis, nbasis * (nbasis + 1)/2 ].
     */
    SharedMatrix df_pmn_;

    /**
     * Inverse of Vpq matrix.
     * Dimension: [nfitbasis, nfitbasis].
     */
    SharedMatrix df_Vpq_inverse_;

    /**
     * AO basis value on grid.
     * dimension: [npts, nbasis].
     */
    SharedMatrix grid_basis_value_;

    /**
     * grid weight.
     * dimension: npts.
     */
    SharedDoubleVector grid_weight_;

    public:
    CurvatureBase(enum DFAType dfa, const SharedMatrix& C_lo, const SharedMatrix& df_pmn,
                  const SharedMatrix& df_Vpq_inverse, const SharedMatrix& grid_basis_value,
                  const SharedDoubleVector& grid_weight);

    virtual SharedMatrix compute() = 0;
};

class CurvatureV1 : public CurvatureBase
{
    private:
    double para_cx_ = 0.930526;
    double para_exf_ = 1.2378;

    SharedMatrix compute_kappa_J();
    SharedMatrix compute_kappa_xc();

    public:
    CurvatureV1(enum DFAType dfa, const SharedMatrix& C_lo, const SharedMatrix& df_pmn,
                const SharedMatrix& df_Vpq_inverse, const SharedMatrix& grid_basis_value,
                const SharedDoubleVector& grid_weight)
        : CurvatureBase(dfa, C_lo, df_pmn, df_Vpq_inverse, grid_basis_value, grid_weight) {}

    virtual SharedMatrix compute() override;
};

class CurvatureV2 : public CurvatureBase
{
    private:
    double para_tau_ = 8.0;
    double para_cx_ = 0.930526;
    double para_exf_ = 1.2378;

    public:
    CurvatureV2(enum DFAType dfa, const SharedMatrix& C_lo, const SharedMatrix& df_pmn,
                const SharedMatrix& df_Vpq_inverse, const SharedMatrix& grid_basis_value,
                const SharedDoubleVector& grid_weight)
        : CurvatureBase(dfa, C_lo, df_pmn, df_Vpq_inverse, grid_basis_value, grid_weight) {}

    virtual SharedMatrix compute() override;
};

}

#endif // _LOSC_CURVATURE_H_

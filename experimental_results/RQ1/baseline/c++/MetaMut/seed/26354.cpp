
#include <Eigen/Dense>

int main()
{
    constexpr std::size_t kColumns{4};
    constexpr std::size_t kRows{4};
    using MyVector = Eigen::Matrix<float, kColumns, 1>;
    using MyMatrix = Eigen::Matrix<float, kColumns, kRows>;

    MyMatrix matrix{MyMatrix::Zero()};
    MyVector const& my_col{matrix.col(0)};
}


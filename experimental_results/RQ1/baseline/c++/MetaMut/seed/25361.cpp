

template <typename T>
constexpr bool IsMatrix = false;

// matrices
template<typename TElem>
class Matrix {};

template <typename TElem>
constexpr bool IsMatrix<Matrix<TElem>> = true;

// Row-vector expanded matrix
template<typename TNestVec>
class RowVecExpMatrix;

template <typename TNestVec>
constexpr bool IsMatrix<RowVecExpMatrix<TNestVec>> = true;

int main()
{
    static_assert(IsMatrix<RowVecExpMatrix<Matrix<int>>>, "Matrix check error");
    static_assert(IsMatrix<Matrix<int>>, "Input type is not a matrix");
}


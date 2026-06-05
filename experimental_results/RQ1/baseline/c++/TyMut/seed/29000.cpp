

template <typename U, long> using CArray = U[];
template <typename U, long Extent, const CArray<U, Extent>> struct X { };

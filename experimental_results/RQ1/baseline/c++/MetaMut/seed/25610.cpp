
template <class _E>   class initializer_list {};
template <int N> struct D {D(std::initializer_list<int>) {}};
D<0> d{1, 2, 3};




template <int N>
struct D {
  ~D() requires(N != 0) = delete;
  ~D() requires(N == 1) = delete;
};

template struct D<0>;

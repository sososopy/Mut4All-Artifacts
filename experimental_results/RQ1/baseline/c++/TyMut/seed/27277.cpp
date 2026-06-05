
struct A
{
  char c;
  constexpr A(int) : c() {}
};

template<int N> struct B
{
  static constexpr auto X = A(N);
};

constexpr A a = B<0>::X;

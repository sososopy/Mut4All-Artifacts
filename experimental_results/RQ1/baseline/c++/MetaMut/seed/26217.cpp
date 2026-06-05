
#include <array>

template<unsigned MaxP, typename Type>
struct test
{
  static constexpr unsigned pole(unsigned P)
  { return P>MaxP? MaxP:P; }

  template<unsigned P>
  using my_array = std::array<Type,pole(P)>;    // causing error

  template<unsigned P>
  void do_something(my_array<P> const&, my_array<P>);
};

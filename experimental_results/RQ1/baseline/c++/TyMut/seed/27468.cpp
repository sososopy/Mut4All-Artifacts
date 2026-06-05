
struct an_array
{
  constexpr bool operator==(const an_array& a) const { return d == a.d; }

  double d;
};

constexpr auto
add_array (an_array a, an_array b)
{
  a.d += b.d;
  return a;
}

template <an_array A>
struct wrapper
{
  static constexpr auto arr = A;
};

constexpr auto ONE       = an_array {{  1. }};
constexpr auto MINUS_ONE = an_array {{ -1. }}; 
constexpr auto MINUS_TWO = an_array {{ -2. }}; 

int
main ()
{
#ifndef FIX
  auto a = wrapper <MINUS_TWO> {};
  __builtin_printf("%f\n", a.arr.d);
#endif
  auto c = wrapper <add_array (MINUS_TWO, ONE)> {};
  __builtin_printf("%f\n", c.arr.d);
  if (c.arr != MINUS_ONE)
    __builtin_abort();
}

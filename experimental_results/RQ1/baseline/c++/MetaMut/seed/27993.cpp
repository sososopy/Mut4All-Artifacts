
template <typename> struct S;
template <typename a> struct T
{
  using A = S<a>;
  using A::b;
  char* __attribute__((aligned(b))) c;
};


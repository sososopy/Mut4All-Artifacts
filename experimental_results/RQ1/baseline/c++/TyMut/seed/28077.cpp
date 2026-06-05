
template <int> struct S
{
  friend void foo (int a = []{ return 0; }()) {}
  int b;
};
S<0> t;



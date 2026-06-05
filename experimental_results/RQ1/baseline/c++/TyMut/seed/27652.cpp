
struct A { unsigned a[32]; };
template <int N>
struct B : public A
{
  static B<N> foo () { B<N> t; t.a[0] = 4; return t; }
};

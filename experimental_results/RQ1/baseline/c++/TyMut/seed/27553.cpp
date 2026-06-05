
template <int _Nm> struct A { typedef int _Type[_Nm]; };
template <int _Nm> struct B { typename A<_Nm>::_Type _M_elems; };
class C;
class D {
  D(C);
};
class F {
public:
  F(B<2>);
  F(D);
};
F fn1() { return {{{0}}}; }

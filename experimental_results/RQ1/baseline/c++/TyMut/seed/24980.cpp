

template<int i> class A {};
template<bool b> class B {};

template<int i> void f(A<i> &, B<bool(i)> &) {}
template<int i> void g(A<i> &, B<static_cast<bool>(i)> &) {} // { dg-warning "mangle" }

int main()
{
  A<1> a;
  B<true> b;
  f(a, b);
  g(a, b);
}


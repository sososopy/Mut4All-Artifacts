
template <int n> class A {};
template <class T> struct B {
  static const int n = 1;
  template <class> A <B<T>::n> f();
  _Complex double c[2], d = 1.0;
};
void g ()
{
  B<int>().f<int>();
}


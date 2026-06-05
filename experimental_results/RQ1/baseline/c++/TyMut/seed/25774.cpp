
template<int> struct A
{
  static const int i;
  template<int N> const int A<N>::i = []{ return 0; }();
};


template<int> struct A {};

template<int N> using B = A<N>;

template<typename T> struct C
{
  B<T::i> b;
};

struct X
{
  static const int i;
};

C<X> c;

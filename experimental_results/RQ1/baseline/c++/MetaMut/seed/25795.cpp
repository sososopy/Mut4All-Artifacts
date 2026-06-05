template <class U>
struct B
{
  typedef void type;
};
template <bool U>
struct C
{
};
template <typename T, typename E = void>
struct D {
  static const bool d = false;
};
template <typename T>
struct D<T, typename B<C<D<T>::d > >::type>
{
  static const bool d = false;
};

D<double> d;

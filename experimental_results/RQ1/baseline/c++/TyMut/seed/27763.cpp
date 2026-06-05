
template <typename> void foo ();
template <typename, typename> struct A;
template <typename T, typename U = decltype(foo<T>)> using B = U;
template <typename T> using C = B<T>;
template <typename T> A<C<T>,int> a;

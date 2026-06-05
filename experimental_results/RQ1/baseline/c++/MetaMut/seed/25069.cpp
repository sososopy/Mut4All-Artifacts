
template <class, int, class, int> struct B {};
template <class T, int a, class U struct B<T, a, U, 1> {};
B<int, 2, char, 1> i;

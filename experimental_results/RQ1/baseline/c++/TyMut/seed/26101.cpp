
template<typename, int> struct A;

template<typename T> struct A<T, T{}> {};

A<int, 0> a;

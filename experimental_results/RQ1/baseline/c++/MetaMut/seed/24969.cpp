
template<class> struct A { };
template<template<class> class, class> struct B { };
template<class, class> struct C { };

template<template<class> class TT, class T, class U> void f(TT<T>, B<TT, T>, U) { }
template<template<class> class TT, class T, class V> void f(TT<T>, B<TT, T>, C<TT<T>, V>) { }

int main()
{
   A<int> a;
   B<A, int> b;
   C<A<int>, long> c;
   f(a, b, c);//ambiguity error
}


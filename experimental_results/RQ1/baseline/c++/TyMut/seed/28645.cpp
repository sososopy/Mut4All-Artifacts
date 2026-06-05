
template<class T> struct A;

template<class T>
struct tuple {
  tuple(T);

  template<template<class> class Tmpl, typename A<Tmpl<char>>::type = 0>
  tuple(Tmpl<T>);

  template<template<class> class Tmpl, typename A<Tmpl<long>>::type = 0>
  tuple(Tmpl<T>);
};

auto r = tuple(0);


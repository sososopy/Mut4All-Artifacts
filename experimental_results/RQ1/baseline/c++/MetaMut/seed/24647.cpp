

  template<typename T> struct X {};
  template<template<typename...> class D, typename ...U> int test(D<U...>*);
  int n = test<X, int>(0);


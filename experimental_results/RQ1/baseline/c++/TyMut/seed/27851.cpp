
template<template<class> class A>
struct G {
    template<class T> using B = A<T>;
    template<B> static int foo();
    int x = foo<42>();
};


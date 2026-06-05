
template<class T> struct B { B(int){} };  explicit B(int) -> B<int>;
template<template<class> class T> void f() { [[maybe_unused]] T _ = 0; }
void g() { f<B>(); }

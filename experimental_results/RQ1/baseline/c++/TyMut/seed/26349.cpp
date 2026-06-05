
template<class T>
void f() {
  [](auto) {
    []<template<class> class L>(L<int>) { };
  };
}

template void f<int>();



template<template <class> class> struct meow {};
template<class T> using kitty = T&;

meow<kitty> u;


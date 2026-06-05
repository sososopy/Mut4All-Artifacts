#include <concepts>

template <auto>
struct A;

template <std::same_as<int> auto p>
struct A<p> {};

A<0> a;
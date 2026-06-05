#include <concepts>

template <auto>
struct A;

template <auto p> requires std::same_as<decltype(p), int*>
struct A<p> {};

template <const int* p>
struct A<p>;

int x = 0;
A<&x> u;
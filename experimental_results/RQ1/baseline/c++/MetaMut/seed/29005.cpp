
#include <concepts>

template <typename T> struct A {
	A(A<T> const&) = default;
};

template <std::same_as<int> T> struct A<T> {
	A(A<T> const&) = default;
};

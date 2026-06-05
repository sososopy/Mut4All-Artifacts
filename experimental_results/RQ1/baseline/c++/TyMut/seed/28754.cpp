
#include <concepts>

struct A{};
struct B : A {};

template <class T> 
concept is_a = std::is_base_of_v<A, T>;

template <class T>
concept is_b = is_a<T> and std::same_as<B, T>;

int foo(is_a auto, is_a auto);

#ifdef OK
int foo(is_b auto, is_a auto);
#else
template <is_a A>
int foo(is_b auto, A);
#endif

A a;
B b;
int d = foo(b, a);


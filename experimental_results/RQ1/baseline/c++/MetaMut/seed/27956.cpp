
template <auto>
struct N{};

template <N>
struct S {};

template <typename T>
using NS = S<T::value>;

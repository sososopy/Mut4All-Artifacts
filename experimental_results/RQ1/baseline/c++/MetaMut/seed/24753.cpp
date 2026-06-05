
#include <cstdlib>

template<template<typename, size_t...> class ThisTT,
            typename T,
            size_t...Dims>
struct S_Base
{
    static constexpr size_t D = (Dims *...);
};

template<template<typename, size_t...> class ThisTT,
            typename T,
            size_t...Dims>
struct S : public S_Base<ThisTT, T, Dims...>
{
    using S_Base<ThisTT, T, Dims...>::D;
    constexpr ThisTT<T, Dims...> f() const
        requires(D > 0) {return {};}

};

template<typename T, size_t M, size_t N>
struct S2 : public S<S2, T, M, N>
{
};

int main(int, char**)
{
    S2<int, 1, 1> s2;
    s2.f();
    return 0;
}

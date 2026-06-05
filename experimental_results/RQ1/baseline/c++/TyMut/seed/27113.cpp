
template<class T>
struct s{};

template<class... T> struct mp_similar_impl;

template<class T> struct mp_similar_impl<T, T>
{
    using type = int;
};

// commenting the following specialization removes the error
template<template<class...> class L, class... T1, class... T2> struct mp_similar_impl<L<T1...>, L<T2...>>
{
    using type = int;
};

int main(){
    mp_similar_impl<s<int>, s<int>>::type v;
}

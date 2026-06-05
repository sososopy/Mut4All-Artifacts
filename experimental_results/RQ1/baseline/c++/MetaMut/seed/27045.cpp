
template<int N>
struct array
{
    int data[N];
};

template<template<typename> class... T1, typename T2>
array<sizeof...(T1)> make_array(T1<T2> ...init)
{
    return { 0 };
}

template<typename T>
struct S
{
    T a;
};

auto arr = make_array(S<int>{1});


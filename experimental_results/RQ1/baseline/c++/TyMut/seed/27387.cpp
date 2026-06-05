
template <class T> struct type { T value; };
template <class T> type(T) -> type<T>;
void f()
{
    []<template <class> class... Ts>()
    {
        (..., []<template <class> class T>() {
            T{0};
        }.template operator()<Ts>());
    }
    .template operator()<type>();
}


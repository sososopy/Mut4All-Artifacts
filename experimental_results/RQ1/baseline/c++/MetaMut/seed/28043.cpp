
template<typename T>
inline constexpr bool is_nontype_list = false;

template<template<auto...> typename T, auto... NonTypes>
inline constexpr bool is_nontype_list<T<NonTypes...>> = true;

template<typename T>
concept bool NonTypeList = is_nontype_list<T>;

// works
template<auto...>
struct A {};

static_assert(is_nontype_list<A<1, 2, 3>>);

// fails
struct X {
    int v;
};

static_assert(is_nontype_list<A<X{1}, X{2}, X{3}>>);

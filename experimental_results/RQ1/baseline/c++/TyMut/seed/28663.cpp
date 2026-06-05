
// value_specialize, work with NTTP only
template<typename Target, template<auto> typename T>
inline constexpr bool is_specialize_value_v = false;
template<template<auto> typename T, auto Ts>
inline constexpr bool is_specialize_value_v<T<Ts>, T> = true;
template<typename Target, template<auto> typename T>
concept specialize_value = is_specialize_value_v<Target, T>;

template<int i> struct Test{};

template<Test str>
struct A
{
    template<typename T> requires specialize_value<T, A>
    int fun(T&&)
    {
        return 1;
    }
};

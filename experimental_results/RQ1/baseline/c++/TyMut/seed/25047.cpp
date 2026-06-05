

template <typename T> struct Container;

template <typename...T>
struct Str1
{
    template <typename...T1>
    struct Helper1
    {
        constexpr static bool value = true;
    };

    template <typename T2, typename...T3>
    struct Helper1<Container<T2>, T3...>
    {
        constexpr static bool value = false;
    };

    static_assert(Helper1<T...>::value, "Check");
};

template <typename...T>
struct Str2
{
    template <typename...T1>
    static constexpr bool Helper2 = true;

    // ERROR here:
    template <typename T2, typename...T3>
    static constexpr bool Helper2<Container<T2>, T3...> = false;

    static_assert(Helper2<T...>, "Check");
};


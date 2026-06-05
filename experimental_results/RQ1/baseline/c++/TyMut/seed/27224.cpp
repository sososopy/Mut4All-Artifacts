
template <int N>
struct fixed_string {
    char content[N] = {};

    constexpr fixed_string(const char (&)[N]) noexcept {}
};

template <int N> fixed_string(const char (&)[N]) -> fixed_string<N>;


template<fixed_string Str>
struct bar
{};

template<fixed_string Str>
struct foo
{
    using type = bar<Str.content>;
};


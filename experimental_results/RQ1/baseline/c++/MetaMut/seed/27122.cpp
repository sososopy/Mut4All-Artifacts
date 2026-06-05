
#include <algorithm>
#include <string_view>

template<std::size_t N>
struct fixed_string
{
    char buf[N + 1]{};
    constexpr fixed_string(char const* s) { std::copy_n(s, N, buf); }
    constexpr operator char const *() const { return buf; }
    constexpr operator std::string_view() const { return buf; }
};
template<std::size_t N>
fixed_string(char const (&)[N]) -> fixed_string<N - 1>;

template<fixed_string Literal>
struct foo;

template<typename T>
struct bar;

template<fixed_string Literal>
struct bar<foo<Literal.buf>>
{};

int main()
{
    bar<foo<"foobar">> foobar;
}

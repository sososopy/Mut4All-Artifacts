
#include <cstdlib>

template<typename CharT, std::size_t N>
struct basic_fixed_string {
    CharT data_[N+1] = {};

    constexpr basic_fixed_string(const CharT (&txt)[N+1]) noexcept
    {
        for(std::size_t i = 0; i <= N; ++i)
            data_[i] = txt[i];
    }
    // auto operator==(const basic_fixed_string &) = default;
};

template<typename CharT, std::size_t N>
basic_fixed_string(const CharT (&str)[N]) -> basic_fixed_string<CharT, N-1>;

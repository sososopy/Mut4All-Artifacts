#include <cstdint>

template<typename CharT, std::size_t N>
struct basic_fixed_string {
  CharT data_[N + 1] = {};

  constexpr basic_fixed_string(CharT ch) noexcept { data_[0] = ch; }

  constexpr basic_fixed_string(const CharT (&txt)[N + 1]) noexcept
  {
    if constexpr (N != 0)
      for (std::size_t i = 0; i < N; ++i) data_[i] = txt[i];
  }

  [[nodiscard]] constexpr const CharT& operator[](std::size_t index) const noexcept { return data_[index]; }
  [[nodiscard]] constexpr CharT operator[](std::size_t index) noexcept { return data_[index]; }

  template<std::size_t N2>
  [[nodiscard]] constexpr friend basic_fixed_string<CharT, N + N2> operator+(
      const basic_fixed_string& lhs, const basic_fixed_string<CharT, N2>& rhs) noexcept
  {
    CharT txt[N + N2 + 1] = {};

    for(std::size_t i = 0; i != N; ++i) txt[i] = lhs[i];
    for(std::size_t i = 0; i != N2; ++i) txt[N + i] = rhs[i];

    return basic_fixed_string<CharT, N + N2>(txt);
  }
};

template<typename CharT, std::size_t N>
basic_fixed_string(const CharT (&str)[N]) -> basic_fixed_string<CharT, N - 1>;

template<typename CharT>
basic_fixed_string(CharT) -> basic_fixed_string<CharT, 1>;

template<std::size_t N>
using fixed_string = basic_fixed_string<char, N>;

template<basic_fixed_string>
struct X {};

template<basic_fixed_string Txt1, basic_fixed_string Txt2>
auto merge(X<Txt1>, X<Txt2>)
{
  return X<Txt1 + Txt2>();
}

auto res = merge(X<"ab">(), X<"cd">());

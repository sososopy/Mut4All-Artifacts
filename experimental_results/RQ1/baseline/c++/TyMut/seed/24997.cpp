

#include <iostream>
#include <array>
#include <utility>

template <typename T, size_t N, size_t... I> constexpr inline std::array<T, N>
make_array_1 (const T (&chr)[N], std::index_sequence<I...>)
{
  return {{ (chr[I])... }};
}

template <typename T, size_t N> constexpr inline std::array<T, N>
make_array (const T (&chr)[N])
{
  return make_array_1 (chr, std::make_index_sequence<N> ());
}

template < unsigned int N, typename T > constexpr inline T reverse_bits (T val)
{
  T out = 0;

  for (unsigned int i = 0; i < N; ++i)
    if (val & (T (1) << i))
      out |= T (1) << (N - i - 1);

  return out;
}



#include <type_traits>

template<unsigned int Nbits>
using uintsz = decltype([](auto i){
  if constexpr (i <= 32)
    return 0;
  else
    return 0L;
 }(std::integral_constant<unsigned int, Nbits>{}));

template<unsigned int Nbytes>
constexpr uintsz<8*Nbytes>
f()
{
  return 0;
}

static_assert(std::is_same_v<decltype(f<7>()), uintsz<56>>);

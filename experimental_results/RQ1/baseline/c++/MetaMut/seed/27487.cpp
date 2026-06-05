
#include <compare>
template <bool B = true>
struct X
{
    auto operator<=>(const X&) const noexcept(B) = default;
};
using Y = decltype(X{} <= X{});

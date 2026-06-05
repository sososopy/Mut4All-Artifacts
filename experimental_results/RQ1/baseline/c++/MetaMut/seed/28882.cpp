
#include<type_traits>
template<auto V>constexpr bool is_const=std::is_const_v<decltype(V)>;
struct A{};
static_assert(!is_const<A{}>);

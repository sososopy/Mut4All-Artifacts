
#include<initializer_list>
constexpr std::initializer_list<bool>il{true};
template<const bool*B>constexpr bool front=B[0];
static_assert(front<il.begin()>);

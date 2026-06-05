
#include <array>
template<typename T> static constexpr void t(std::initializer_list<int> x) { std::array<int, x.size()> v; }



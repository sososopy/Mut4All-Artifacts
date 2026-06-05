
#include <tuple>

static_assert(std::is_same<decltype(std::get<0>(std::declval<std::tuple<int> const &&>())), int const &&>::value);

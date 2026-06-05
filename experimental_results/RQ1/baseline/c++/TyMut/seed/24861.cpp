
#include <type_traits>

static_assert(!std::is_array_v<int[0]>);

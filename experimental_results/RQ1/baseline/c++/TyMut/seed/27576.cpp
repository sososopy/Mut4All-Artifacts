
#include <type_traits>

template<typename T>
using id_t = T;

static_assert(std::is_same_v<decltype(id_t<void const>()), void>);


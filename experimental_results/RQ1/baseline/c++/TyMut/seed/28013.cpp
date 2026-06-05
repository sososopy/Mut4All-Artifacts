
#include <utility>

template <typename T, typename = void>
struct check_concept : std::false_type {};

template <typename T>
struct check_concept<T, std::enable_if_t<
        std::is_same_v<decltype(f(std::declval<T>())), int>>> : std::true_type {};


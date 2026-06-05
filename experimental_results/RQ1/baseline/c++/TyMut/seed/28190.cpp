
#include <string>

template <typename T, typename U>
constexpr inline bool isAddable = requires(const T& lhs, const U& rhs) {
     lhs + rhs;
};

auto x = isAddable<int, std::string>;


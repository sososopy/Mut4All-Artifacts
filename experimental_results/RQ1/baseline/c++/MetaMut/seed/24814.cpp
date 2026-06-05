
#include <type_traits>

template<int ...Is>
constexpr int my_array[]{Is...};

static_assert(std::is_same_v<decltype(my_array<1, 2, 3, 4, 5>), const int[5]>); // rejected since GCC13
static_assert(sizeof(my_array<1, 2, 3, 4, 5>) == sizeof(int) * 5); // rejected since GCC13

static_assert(std::is_same_v<decltype(my_array<1, 2, 3, 4, 5>), const int[]>); // passes since GCC13, but should not


#include <vector>

[[nodiscard]] constexpr std::vector<std::vector<int>> constexpr_get_data() {
    return std::vector<std::vector<int>>{{1, 2}, {3, 4}};
}

[[nodiscard]] consteval std::vector<std::vector<int>> consteval_get_data() {
    return std::vector<std::vector<int>>{{1, 2}, {3, 4}};
}

[[nodiscard]] constexpr auto get_val(
    const std::vector<std::vector<int>> list_of_lists) {
    return list_of_lists.data()[0];
}

static_assert(std::vector<int>{1, 2} == get_val(std::vector<std::vector<int>>{
                                            {1, 2}, {3, 4}}));  // Failing
static_assert(std::vector<int>{1, 2} ==
              get_val(consteval_get_data()));  // Failing
static_assert(std::vector<int>{1, 2} ==
              get_val(constexpr_get_data()));  // Working



#include <functional>
static_assert(not std::ranges::equal_to{}([]{}, +[]{}));
static_assert(std::not_equal_to{}([]{}, +[]{}));
static_assert([]{} != +[]{});

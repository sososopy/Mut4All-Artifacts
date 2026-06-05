
#include <cstddef>
static constexpr std::size_t alignment = 256;
struct alignas(alignment) foo {
  char c;
};
static_assert(alignof(foo) == alignment, "");


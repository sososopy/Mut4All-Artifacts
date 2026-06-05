
#include <cstddef>
#include <cstdlib>

template <typename B, size_t N>
constexpr bool test(B&& b, char const (&reason)[N]) {
  [[unlikely]] if (not static_cast<bool>(b)) {
    (void)reason;
    std::abort();
  }
  return true;
}

static_assert( test( true, "testing" ) )

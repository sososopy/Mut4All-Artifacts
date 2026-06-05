
#include <type_traits>
#include <vector>
#include <utility>

template <typename T>
struct span {
  using value_type = T;
  span(std::vector<T> &x) {}
};

template <typename X>
using SpanType = decltype(span(std::declval<X>()));

// Compilation succeeds if we change line below to #if 1
#if 0
template <typename X>
using ConstSpanType =
    span<const typename decltype(span(std::declval<X>()))::value_type>;
#else
template <typename X>
using ConstSpanType = span<const typename SpanType<X>::value_type>;
#endif

static_assert(
    std::is_same_v<ConstSpanType<std::vector<int>&>, span<const int>>);




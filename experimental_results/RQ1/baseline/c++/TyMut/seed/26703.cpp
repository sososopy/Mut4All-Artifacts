
#include <stl2/detail/iterator/concepts.hpp>

template <class T>
concept bool R = requires (T& t) {
  { t.begin() } -> stl2::Iterator;
  { t.end() } -> stl2::Sentinel<decltype(t.begin())>;
};

struct foo {
  int* begin();
  int* end();
};

R{T}
constexpr bool f() { return true; }

static_assert(f<foo>());


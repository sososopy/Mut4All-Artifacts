
#include <string_view>

template <class>
constexpr bool g() {
  std::string_view s = " ";
  s.find(" ");
  return true;
}

constexpr bool b = g<void>();          // ok
template <class T> concept C = g<T>();
static_assert(C<void>);                // error

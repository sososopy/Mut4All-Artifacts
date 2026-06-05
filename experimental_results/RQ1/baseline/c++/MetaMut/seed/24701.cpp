
#include <type_traits>
#include <ostream>

using trait = std::is_constructible<std::ostream, const std::ostream&>;

#if B
const bool dummy = trait::value;
#endif

#if A
template<bool> struct X { };

struct moveable_class
{
  template <typename Source>
  moveable_class(Source&& src, X<trait::value>* = 0)
  { }
};
#endif

static_assert( !trait::value, "");

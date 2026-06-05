
class locale
{
  template<typename _Cache>
  friend struct __use_cache;
};
// pr99248.a.H
#include "pr99248.h"
// pr99248_b.H
#include "pr99248.h"
import "pr99248_a.H";

template<typename _Facet>
struct __use_cache;


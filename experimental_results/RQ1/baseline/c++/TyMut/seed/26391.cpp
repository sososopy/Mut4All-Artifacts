
#include <type_traits>

using std::is_same_v;

struct s0 {};
struct s1 {};
template <typename _Key> 
struct s3 {
  template <typename _Up, typename _Vp = _Up>
  static constexpr bool __usable_key = is_same_v<_Vp, _Key>;
  static_assert(!__usable_key<s1>);
};
s3<s0> t;


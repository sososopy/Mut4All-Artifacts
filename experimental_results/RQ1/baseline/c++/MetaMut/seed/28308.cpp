template<class>
constexpr bool is_hello = false;

template <typename T>
  requires (!is_hello<T>)
class Hello;

template <typename T>
  requires (!is_hello<T>)
class Hello {};

static_assert(requires { []<Hello<int>>{}; });


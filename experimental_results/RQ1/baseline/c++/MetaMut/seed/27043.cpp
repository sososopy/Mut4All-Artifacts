
template <class>
  requires false
constexpr bool v = true;

template <class>
constexpr bool f() { return true; }

template <class T>
  requires v<T>
constexpr bool f() { return false; }

static_assert(f<void>());





#include <tuple>

template <typename> using whatever = void;

template<typename, typename = void> struct has_f;

template <typename T>
struct has_f<T, whatever<decltype( &T::f )>> {};

struct X {
  void f() const {}
};

using t = has_f<std::tuple<X> >::asdf;


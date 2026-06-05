
namespace N {
  template <typename, typename>
  bool c;
}
namespace {
class A {};
class B {};
using D = class C {};
class E {};
}
namespace O::P::Q {
struct F {};
}
namespace O::R {
template <typename>
struct G;
template <typename T>
struct H {
  using I = G<decltype (T::foo)>;
  constexpr static unsigned h = I::cr;
};
template <typename T>
A
bar ()
{
  if (N::c<T, P::Q::F>) {
    struct J {
      static void foo();
    };
    H<J>();
  }
  return A ();
}
template <typename T>
A
baz ()
{
  bar<T>;
  return A ();
}
}
namespace O::P {
template <typename T>
A
baz (B, D, C, E, T)
{
  R::baz<T>;
  return A ();
}
A
baz (B cx, D cy, C cz, E d)
{
  baz (cx, cy, cz, d, Q::F{});
  return A ();
}
}

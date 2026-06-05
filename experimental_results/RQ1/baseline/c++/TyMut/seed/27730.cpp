
namespace std {
template <bool> struct enable_if;
}
template <typename T> using _t = typename T::type;
struct A {
  using type = int;
};
template <template <typename> class> A try_defer_(int);
template <template <typename> class C>
using defer_ = decltype(try_defer_<C>(0));
template <template <typename> class C> struct F : defer_<C> {};
struct B {
  template <typename> using invoke = int;
};
template <typename...> using and_ = _t<F<B::invoke>>;
template <typename> struct C;
struct {
  template <typename Fun> C<Fun> operator()(Fun);
} a;
template <typename> struct C {
  template <typename> using ViewConcept = and_<>;
  template <typename Rng, typename std::enable_if<(ViewConcept<Rng>())>::type>
  auto m_fn1() -> decltype(a(0)) {}
};
struct D;
C<D> b;




#include <memory>
struct A {};
template <class...> struct typelist {};
template <class... Cs> std::unique_ptr<A> chooseB(typelist<Cs...>);
template <class... Cs, class Idx, class... Rest>
std::unique_ptr<A> chooseB(typelist<Cs...> choices, Idx, Rest... rest) {
  auto f = [=](auto) { return [=] { return chooseB(choices, rest...); }; };
  std::function<std::unique_ptr<A>()> fs[]{f(Cs{})...};
}
main() { chooseB(typelist<double, char>{}, 0, 1, 2); }

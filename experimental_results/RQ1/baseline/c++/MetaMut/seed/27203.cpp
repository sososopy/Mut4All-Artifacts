
template <int __v> struct integral_constant {
  static constexpr int value = __v;
};
template <bool, class> using enable_if_t = int;
struct tmp {
  template <class> static constexpr auto is_integral() -> bool;
  template <class E>
  static auto func(E, E) -> enable_if_t<tmp::is_integral<E>(), bool>;
};
template <class> constexpr auto tmp::is_integral() -> bool {
  return integral_constant<false>::value;
}
int main() { tmp::func(1, 0); }


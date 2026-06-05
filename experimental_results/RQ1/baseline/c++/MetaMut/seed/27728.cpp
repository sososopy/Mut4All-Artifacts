
#include <functional>
template <typename I>
concept bool input_or_output_iterator = std::is_same_v<I &, decltype(++I{})>;
template <class From, class To> concept convertible_to = requires(From(f)()) {
  static_cast<To>(f());
};
template <typename> struct reference;
template <typename t> using reference_t = typename reference<t>::type;
template <input_or_output_iterator it_t> struct reference<it_t> {
  using type = typename std::iterator_traits<it_t>::reference;
};
template <typename rng_t>
requires (!input_or_output_iterator<rng_t>) struct reference<rng_t> {
  using type = reference_t<decltype(begin(std::declval<rng_t &>()))>;
};
template <typename> struct b {
  template <typename f>
  static constexpr bool e = convertible_to<reference_t<f>, int>;
  template <typename f> b(std::initializer_list<f>) requires e<f>;
};
b<void> g{std::vector<int>{}};

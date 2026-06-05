
#include <functional>
#include <iostream>
namespace ranges {
struct view_interface {
  template < bool = true > operator bool();
#if 1
  template < bool B = true> requires false operator bool(); // ICE on gcc <= 10
#elif 1
  template < bool B = true> requires false explicit operator bool(); // ICE on gcc <= 8. This code is working since gcc 9.
#else
  template < bool B = true, std::enable_if_t<B>> explicit operator bool(); // a possible workaround
#endif
};

template < typename View > struct impl {
  template < typename... Ts, typename V = View >
  static auto bind(Ts... ts) -> decltype(V::bind((ts)...));
};

template < typename > struct view;
template < typename Fun > view< Fun > make_view(Fun);
template < typename View > struct view {
  template < typename Arg, typename Pipe >
  friend auto operator|(Arg, Pipe)
      -> decltype(Pipe::pipe(std::declval< Arg >, std::declval< Pipe >()));
  View view_;
  template < typename Rng, typename Vw > static auto pipe(Rng, Vw v) {
    return v.view_(0);
  }
  template < typename... Ts, typename V = View >
  auto operator()(Ts... ts)
      -> decltype(make_view(impl< V >::bind(view_, (ts)...)));
};
struct f {
  template < typename g > static auto bind(f h, g t) {
    return std::bind(h, std::placeholders::_1, t);
  }
  template < typename a, typename ValRng >
  auto operator()(a, ValRng) -> view_interface;
};
view< f > join;
} std::string e() {
  std::vector< std::string > extensions;
  std::string i;
  auto o = extensions | ranges::join(i);
  std::cout << o;
}


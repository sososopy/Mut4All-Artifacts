
namespace std {
template <typename _Tp, _Tp __v> struct A { static constexpr _Tp value = __v; };
template <bool __v> using __bool_constant = A<bool, __v>;
auto declval();
struct B {
  template <typename> static __bool_constant<noexcept(declval)> __test(int);
};
template <typename _Tp> struct __is_nt_destructible_impl : B {
  typedef decltype(__test<_Tp>(0)) type;
};
template <typename>
struct is_nothrow_destructible : __is_nt_destructible_impl<int>::type {};
} // namespace std
namespace meta {
namespace detail {
template <class T, T> struct require_constant;
}
template <typename T> concept trait = requires {
  typename detail::require_constant<decltype(T()), {}>;
};
template <trait T> using _t = T::type;
} // namespace meta

template <typename T>
concept destructible = std::is_nothrow_destructible<T>::value;
template <typename T> concept constructible_from = destructible<T>;
template <typename T> concept default_constructible = constructible_from<T>;
namespace ranges {
template <typename T> struct static_const { static constexpr T value{}; };
namespace detail {
int as_const;
template <typename> using as_const_t = decltype(as_const);
} // namespace detail
template <typename> struct G;
namespace detail {
template <typename> struct C;
}
template <typename I> using reverse_iterator = G<detail::C<I>>;
struct range_access {
  template <typename T> static T::mixin mixin_base_2_(int);
  template <typename Cur> struct mixin_base_ {
    using type = decltype(mixin_base_2_<Cur>(2));
  };
  template <typename Cur> using mixin_base_t = meta::_t<mixin_base_<Cur>>;
  auto read();
};
template <typename> concept readable_cursor = requires { range_access::read; };
template <typename T> struct basic_mixin {
  basic_mixin() requires default_constructible<T>;
};
namespace detail {
template <typename Cur, bool> struct H : range_access::mixin_base_t<Cur> {
  using range_access::mixin_base_t<Cur>::mixin_base_t;
};
template <typename Cur>
using iterator_associated_types_base = H<Cur, readable_cursor<Cur>>;
} // namespace detail
template <typename Cur>
struct G : detail::iterator_associated_types_base<Cur> {};
namespace detail {
template <typename> struct C {
  struct mixin : basic_mixin<C> {
    using basic_mixin<C>::basic_mixin;
  };
};
} // namespace detail
namespace _rbegin_ {
struct fn {
  struct D {
    template <typename R> auto operator()(R r) noexcept(noexcept(r.rbegin())) {}
  };
  template <typename> using impl = D;
  template <typename R> auto operator()(R r) noexcept(noexcept(impl<R>{}(r))) {}
};
template <typename> using _t = decltype(fn{});
} // namespace _rbegin_
auto rbegin = static_const<_rbegin_::fn>::value;
namespace _crbegin_ {
struct fn {
  template <typename R>
  _rbegin_::_t<detail::as_const_t<R>>
  operator()(R r) noexcept(noexcept(rbegin(r)));
};
} // namespace _crbegin_
auto crbegin = static_const<_crbegin_::fn>::value;
} // namespace ranges

struct F {
  using value_type = int;
  using const_iterator = value_type;
  using const_reverse_iterator = ranges::reverse_iterator<const_iterator>;
  const_reverse_iterator rbegin();
};
template <typename Sequence1234> auto test_crits(Sequence1234 a) {
  ranges::crbegin(a);
}

auto test_array() {
  F a;
  test_crits(a);
}


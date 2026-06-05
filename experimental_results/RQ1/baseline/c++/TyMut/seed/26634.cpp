
namespace std {
template <typename _Tp> struct integral_constant {
  static constexpr _Tp value = 0;
};
template <bool, typename, typename> struct conditional;
template <typename...> struct __or_;
template <typename _B1, typename _B2>
struct __or_<_B1, _B2> : conditional<1, _B1, _B2> {};
template <typename...> struct __and_;
template <typename _B1, typename _B2>
struct __and_<_B1, _B2> : conditional<1, _B2, _B1> {};
template <typename> struct is_rvalue_reference;
template <typename _Tp>
struct is_reference
    : __or_<integral_constant<bool>, is_rvalue_reference<_Tp>>::type {};
template <typename _Tp> _Tp declval();
struct __do_is_direct_constructible_impl {
  template <typename _Tp, typename _Arg,
            typename = decltype(_Tp(declval<_Arg>()))>
  static integral_constant<bool> __test(int);
};
template <typename _Tp, typename _Arg>
struct __is_direct_constructible_impl : __do_is_direct_constructible_impl {
  decltype(__test<_Tp, _Arg>(0)) type;
};
template <typename _Tp, typename _Arg>
struct __is_direct_constructible_new_safe
    : __and_<int, __is_direct_constructible_impl<_Tp, _Arg>> {};
template <typename _Tp, typename _Arg>
struct __is_direct_constructible_new
    : conditional<is_reference<_Tp>::value, int,
                  __is_direct_constructible_new_safe<_Tp, _Arg>> {};
template <typename _Tp, typename _Arg>
struct __is_direct_constructible
    : __is_direct_constructible_new<_Tp, _Arg>::type {};
template <typename...> struct __is_nary_constructible;
template <typename _Tp, typename _Arg>
struct __is_nary_constructible<_Tp, _Arg>
    : __is_direct_constructible<_Tp, _Arg> {};
template <typename _Tp, typename... _Args>
struct is_constructible : __is_nary_constructible<_Tp, _Args...>::type {};
template <bool> struct enable_if;
template <bool, typename _Iftrue, typename> struct conditional {
  typedef _Iftrue type;
};
template <typename _Iftrue, typename _Iffalse>
struct conditional<false, _Iftrue, _Iffalse> {
  typedef _Iffalse type;
};
}
namespace boost {
namespace hana {
template <bool> struct fast_and;
template <typename... Xn> struct closure {
  template <typename... Yn,
            typename = std::enable_if<
                fast_and<std::is_constructible<Xn, Yn &&>::value...>::value>>
  closure(Yn...);
  template <typename... Yn,
            typename std::enable_if<
                fast_and<std::is_constructible<Yn>::value...>::value>::type>
  closure(closure<Yn...>);
};
template <typename> struct make_impl;
template <typename Datatype> struct make_t {
  template <typename... X> decltype(auto) operator()(X... x) {
    return make_impl<Datatype>::apply(x...);
  }
};
template <typename Datatype> make_t<Datatype> make;
struct Pair;
auto make_pair = make<Pair>;
struct Tuple;
auto make_tuple = make<Tuple>;
template <> struct make_impl<Tuple> {
  template <typename... Xs> static closure<Xs...> apply(Xs &&...);
};
struct pair : closure<> {
  using closure::closure;
};
template <> struct make_impl<Pair> {
  template <typename F, typename S> static pair apply(F, S);
};
namespace struct_detail {
template <typename Memptr, Memptr> struct member_ptr {};
template <int, typename> auto prepare_member_name_impl() {}
template <int n, typename> auto prepare_member_name() {
  return prepare_member_name_impl<n, int>;
}
}
}
}
struct Person {
  int Person;
  auto apply() {
    struct member_names;
    boost::hana::make_tuple(boost::hana::make_pair(
        boost::hana::struct_detail::prepare_member_name<0, member_names>(),
        boost::hana::struct_detail::member_ptr<decltype(&Person::Person),
                                               &Person::Person>{}));
  }
};


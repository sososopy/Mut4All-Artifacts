
namespace std {
template <typename _Tp, _Tp> struct A;
template <typename, typename> struct B;
template <bool> struct enable_if;
template <typename> class allocator;
template <typename = allocator<int>> class vector {};
}
namespace meta {
template <typename T, T...> struct C;
template <bool B> using bool_ = std::A<bool, B>;
template <typename> struct D;
template <template <typename> class> struct quote;
template <bool... Bools> using and_c = std::B<C<bool, Bools...>, C<bool>>;
template <typename> using fast_and = and_c<>;
}
template <typename I, typename> using common_iterator = I;
template <typename T> struct static_const { static constexpr T value{}; };
template <typename> int _nullptr_v;
template <typename Concept>
struct F : meta::bool_<decltype(_nullptr_v<Concept>)::value> {};
template <typename...> using Constructible = struct View;
template <typename> using range_iterator_t = View;
template <typename> using range_sentinel_t = View;
template <typename Rng>
using range_common_iterator_t =
    common_iterator<range_iterator_t<Rng>, range_sentinel_t<Rng>>;
template <typename, typename I> using ReserveAndAssignable = F<I>;
template <typename Rng, typename = range_common_iterator_t<Rng>>
using ConvertibleToContainer = meta::fast_and<Constructible<>>;
template <typename> struct to_container_fn {
  template <typename C, typename R>
  using ReserveConcept = meta::fast_and<ReserveAndAssignable<C, R>>;
  template <typename Rng, typename Cont,
            typename std::enable_if<ConvertibleToContainer<Cont>() &&
                                    ReserveConcept<Cont, Rng>()>::type>
  void m_fn1();
};
auto to_vector = static_const<to_container_fn<meta::quote<std::vector>>>::value;
template <typename Cont> to_container_fn<meta::D<Cont>> to_();
struct G {
  operator std::vector<std::allocator<int>>() { to_<std::vector<>>(); }
};


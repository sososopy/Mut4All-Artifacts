
template <typename _Tp>
using remove_reference_t = _Tp;
template <typename _Tp>
using remove_extent_t = _Tp;
template <typename _Tp, _Tp>
struct integer_sequence {};
template <typename _Tp, _Tp _Num>
using make_integer_sequence = integer_sequence<_Tp, __integer_pack(_Num)...>;
template <int... _Idx>
using index_sequence = integer_sequence<unsigned, _Idx...>;
template <int _Num>
using make_index_sequence = make_integer_sequence<unsigned, _Num>;
auto bind(auto... a) {
  [&]<auto... J>(index_sequence<J...>) {
    ([a] {
      using A = remove_reference_t<decltype(a)>;
      is_same_v<remove_extent_t<A>>(J);
    } ||
     ...);
  }
  (make_index_sequence<sizeof...(a)>());
}
auto operator_sq2(bind(1.3));

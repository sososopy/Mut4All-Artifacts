
template <typename _Tp, _Tp> struct integral_constant {};
template <typename _Tp, _Tp> struct integer_sequence {};
template <typename _Tp, _Tp _Num>
using make_integer_sequence = integer_sequence<_Tp, __integer_pack(_Num)...>;
template <long... _Idx>
using index_sequence = integer_sequence<unsigned long, _Idx...>;
template <long N>
using make_index_sequence = make_integer_sequence<unsigned long, N>;
namespace sequence_tuple {
struct tuple {};
template <int> auto get(tuple t) { return t; }
} // namespace sequence_tuple
template <long Index> using size_t_ = integral_constant<unsigned long, Index>;
template <class T> auto tie_as_tuple(T, size_t_<1>) {
  sequence_tuple::tuple __trans_tmp_1;
  return __trans_tmp_1;
}
template <class, unsigned long N>
constexpr auto detect_fields_count_dispatch(size_t_<N>) {
  return 1;
}
template <class> constexpr long fields_count() {
  long result = detect_fields_count_dispatch<int>(size_t_<0>{});
  return result;
}
template <class T> auto tie_as_tuple(T val) { return tie_as_tuple(val, {}); }
void get();
template <class T, class F> void for_each_field(T value, F func) {
  constexpr long fields_count_val = fields_count<T>();
  auto t = tie_as_tuple(value);
  [&]<unsigned long... I>(index_sequence<I...>) { (func(get<I>(t)), ...); }
  (make_index_sequence<fields_count_val>{});
}
void index_in_struct(auto s, auto... member) {
  for_each_field(s, [&](auto) {
    if constexpr (requires { (... * member); })
      (... * member);
  });
}
void z() { index_in_struct(int{}, z); }

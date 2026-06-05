
module;
# 1 "" 1
template <typename _Derived, typename _Base>
concept derived_from = __is_base_of(_Base, _Derived);
namespace ranges {
int swap;
}
template <typename> concept swappable = requires { ranges::swap; };
template <typename _Tp> concept movable = swappable<_Tp>;
namespace __detail {
template <typename _Tp, typename>
concept __weakly_eq_cmp_with = requires(_Tp __t) {
  __t;
};
} // namespace __detail
template <typename> struct iterator_traits;
struct input_iterator_tag;
namespace __detail {
template <typename _Iter> using __iter_concept = _Iter;
}
template <typename _Iter> concept weakly_incrementable = movable<_Iter>;
template <typename _Iter>
concept input_or_output_iterator = weakly_incrementable<_Iter>;
template <typename _Sent, typename _Iter>
concept sentinel_for = __detail::__weakly_eq_cmp_with<_Sent, _Iter>;
template <typename _Iter>
concept input_iterator =
    derived_from<__detail::__iter_concept<_Iter>, input_iterator_tag>;
template <typename> struct iterator;
template <typename _Iterator>
class reverse_iterator
    : iterator<typename iterator_traits<_Iterator>::value_type> {};
template <input_or_output_iterator _It, sentinel_for<_It>>
class common_iterator;
template <input_iterator _It, typename _Sent>
struct iterator_traits<common_iterator<_It, _Sent>>;
class basic_string_view {
  using value_type = int;
  using const_iterator = value_type;
  using const_reverse_iterator = reverse_iterator<const_iterator>;
};
# 3 "" 2
export module hello;
void greeter(basic_string_view);


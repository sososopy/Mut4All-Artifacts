
template <typename _Tp, _Tp __v> struct A { static constexpr _Tp value = __v; };
typedef A<bool, false> false_type;
template <typename> struct DependentFalse : false_type {};
enum class E;
template <E id> void fn1() {
  static_assert(DependentFalse<decltype(id)>::value);
}

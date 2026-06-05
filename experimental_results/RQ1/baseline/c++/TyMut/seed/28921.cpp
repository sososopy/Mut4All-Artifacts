
namespace std {
template <typename _Tp, int _Nm> struct __array_traits {
  typedef _Tp _Type[_Nm];
};
template <typename _Tp, int _Nm> struct array {
  __array_traits<_Tp, _Nm>::_Type _M_elems;
};
} // namespace std
struct scope_guard {
  constexpr scope_guard(scope_guard &&) {}
  template <int count> static auto array(void()) {
    std::array<scope_guard, count> r;
    return r;
  }
  scope_guard() = default;
};
void test() { scope_guard::array<2>(test); }

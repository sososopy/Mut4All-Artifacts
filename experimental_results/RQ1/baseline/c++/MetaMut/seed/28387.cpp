
template <typename _Tp> _Tp __declval(long);
template <typename _Tp> auto declval() -> decltype(__declval<_Tp>(0));
template <typename _Tp> _Tp forward;
template <class A, class B> auto mul(A a, B) -> decltype(a * 0);
namespace types {
struct str {};
} // namespace types
void operator*(types::str, long);
namespace anonymous {
template <class T> types::str str(T);
}
struct str {
  template <typename... Types>
  auto operator()(Types...) -> decltype(anonymous::str(forward<Types>...));
};
typedef decltype(mul(declval<decltype(str()(0))>(), 0)) __type10;


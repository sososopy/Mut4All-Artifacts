
namespace std {
template <int __v> struct A {
  typedef bool value_type;
  constexpr operator value_type() { return __v; }
};
template <typename> struct is_lvalue_reference : A<false> {};
} // namespace std
template <class T> void fn1(T) requires !std::is_lvalue_reference<T>();
template <class T> void fn1(T);
void fn2() { fn1(fn2); }

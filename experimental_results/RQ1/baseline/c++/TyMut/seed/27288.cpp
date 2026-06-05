
constexpr int b = false;
struct T {
  template <class> struct S
  { S () noexcept (b) {} };
  int a = __has_nothrow_constructor (S<int>);
};

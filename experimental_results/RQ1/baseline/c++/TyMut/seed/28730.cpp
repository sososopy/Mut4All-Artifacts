
template <class T>
struct A {
  template <class U>
  static auto f() {
    struct S{};
    return S{};
  }
};

using type = void;
using type = decltype(A<int>::f<char>()); // A<int>::f<char>()::S


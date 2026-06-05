
template <typename T> T foo ();
template <typename> using A = int;
template <typename T, typename U>
auto operator| (T, U) -> decltype (U() (T()));
template <typename T> struct B {
  template <typename U, typename = A<decltype (foo<T> () (0, foo<typename U::E> ()))>>
  void operator() (U);
};
struct {
  template <typename T, typename U>
  B<T> operator() (T, U) { return B<T> (); }
} c;
struct D {
  D() {
    c([] {}, 0);
  }
  struct E {
  };
  void bar ()
  {
    E f;
    f | c ([] (int, E) {}, 0);
  }
};

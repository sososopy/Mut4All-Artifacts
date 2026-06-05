
template <typename T>
struct foo {
  template <T *P> static void f() {
  }
  static void *g() {
    return (void*)f<nullptr>;
  }
};

inline void *f() {
  struct S {
  };
  return foo<S>::g();
}

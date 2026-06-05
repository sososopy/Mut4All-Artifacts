
struct A {
  template<int N> void f();
  auto g() -> decltype(this->f<0>());
};


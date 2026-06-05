

template<int>
struct S {
  template<int> static void f();
  S() { void(*g)(char) = [](auto) { f<0>; }; }
};
S<0> s;

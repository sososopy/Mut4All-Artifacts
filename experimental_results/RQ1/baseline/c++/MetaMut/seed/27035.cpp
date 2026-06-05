

  void f(int);
  void f(float);
  template<typename T, T F, T G, bool b = F == G> struct X {};
  template<typename T> void test() { X<void(T), f, f>(); }
  int main() { test<int>(); }


template<class T> struct A {
  auto f() {}
  void g() { f(); }
};


int main() {
  A<void> a;
  a.g();
  return 0;
}


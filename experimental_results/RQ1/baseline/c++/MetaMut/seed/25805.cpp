template <class T> void foo(const T &t) {
  t.foo();
}

template <class F> void bar(F *f) {
  struct B {
    F *fn;
    void foo() const {
      fn();
    }
  } b = { f };
  foo(b);
}

void go() {}

void test() {
  bar(go);
}

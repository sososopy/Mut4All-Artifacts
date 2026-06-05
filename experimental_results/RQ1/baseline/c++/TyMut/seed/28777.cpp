

export module foo;

struct A { ~A() { } };

export inline void f() {
  static thread_local A a;
}

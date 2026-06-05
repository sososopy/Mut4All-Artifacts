struct S {};
struct T {
  void operator delete(S) {}
};

void foo(T *t) { delete t; }
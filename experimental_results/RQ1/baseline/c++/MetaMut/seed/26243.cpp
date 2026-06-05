
struct S {
  S();
  ~S();
  int a, b;
};

void foo(S s) {
  // ~S() is called before return  
}

void bar() {
  S t;
  foo(t);  // S(const S &) is called on t.
}


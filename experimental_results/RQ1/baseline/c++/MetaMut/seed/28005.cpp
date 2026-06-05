
struct P {
  int a = 0;
  void* b;
};

void f(P) {}

void test() {
  f({.b = nullptr});
}


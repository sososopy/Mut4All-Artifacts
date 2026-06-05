
extern "C" int printf(const char *, ...);

struct A {
  void f();
  void operator()(int) { printf("Hello, world!"); }
};

void A::f() {
  const auto &f = [this](auto x) -> void {
    operator()(x);
  };
  f(0);
}

int main(void) {
  A a;
  a.f();
}


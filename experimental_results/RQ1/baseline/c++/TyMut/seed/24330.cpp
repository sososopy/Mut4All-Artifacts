
#define NO_ASAN __attribute__((no_sanitize_address))

class X {
  void f();
  NO_ASAN void g();
};


NO_ASAN void X::f() {
  ...
}

void X::g() {
  ...
}

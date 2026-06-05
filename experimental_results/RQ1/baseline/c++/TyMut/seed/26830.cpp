
struct A {
  operator double() {
    return 33.0;
  }
};

A f() {
  return {};
}

void g() {
  double x = _Cilk_spawn f();
}

int main() {
  g();
  return 0;
}

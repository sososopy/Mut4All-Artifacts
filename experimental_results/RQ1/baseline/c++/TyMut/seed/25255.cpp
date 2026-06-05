
struct A {};

void f(A) {}

void g() {
  _Cilk_spawn f(A{});
}

int main() {
  return 0;
}

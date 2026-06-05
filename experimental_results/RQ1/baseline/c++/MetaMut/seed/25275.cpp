
struct S {};

template <class T = S>
void foo(T t = {}) {}

int main() {
  foo();
}

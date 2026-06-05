
struct S {
  explicit S(int, int, int) {}
};

struct T {
  T(int, int, int) {}
};

void foo(struct S) {} // 1
void foo(struct T) {} // 2

int main() {
  S s{1, 2, 3};

  foo({1, 2, 3});
}



int main() {

  struct A {
    static void test(const A &) {}
    A() {}
    private: A(const A &) {}
  };

  A::test(A());

  return 0;
}


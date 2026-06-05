

struct A {
  A(int a = 0) { }
};

struct B : A {
  using A::A;
  B(int, int);
};

B b;



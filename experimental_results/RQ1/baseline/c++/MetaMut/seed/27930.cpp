struct S {
  int a;
  static int foo() noexcept(noexcept(a));
};

is compiled fine, but 'this' cannot be used in a static member function declaration.  This one we handle ok:

struct S2 {
  int a;
  static int foo() noexcept(noexcept(this->a));
};

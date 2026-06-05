
namespace A {
struct foo {
  void func();
};
namespace B {
struct bar {
  friend class foo;
private:
  int i;
};
}
}

void A::foo::func() {
  A::B::bar b;
  b.i = 3;
}

int main() {
}



struct Foo { Foo() {} };

struct Bar : Foo {
  using Foo::Foo;
  Bar(void*); // Define another constructor so Foo() isn't generated.
};

int main() {
 Bar f;
}

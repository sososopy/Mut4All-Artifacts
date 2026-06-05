
 struct Foo {};
 int f(Foo *p);
 int n = f(&(Foo() = Foo()));


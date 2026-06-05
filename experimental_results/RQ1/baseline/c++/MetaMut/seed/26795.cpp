
enum class A { foo };
void fn1(const A test) {
  switch (test)
  case A::foo: ;
}


class A;
class B;

void f(const B &b) {
  const A &a = static_cast<const A&>(b);
}

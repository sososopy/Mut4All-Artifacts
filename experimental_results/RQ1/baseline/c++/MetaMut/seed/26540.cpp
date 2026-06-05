
struct A { int m; };
struct B { A a; };

template<int A::*P>
void f(B* b) {
  b->a.*P;
}

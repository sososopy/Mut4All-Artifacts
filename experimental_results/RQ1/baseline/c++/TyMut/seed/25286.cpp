struct Empty { };
struct A { A() : a(true) { } bool a; ~A() { if (!a) __builtin_abort(); } };
struct B : Empty { B() : Empty() { } ~B() { } };
struct C : A, B { C() : A(), B() { } ~C() { } };
int main() {
  C c;
}


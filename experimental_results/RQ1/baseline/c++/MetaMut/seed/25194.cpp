
struct A {
  A(int);
};
struct B : A {
  B();
} A;
struct C : virtual A {
  C(int);
};
A::A(int p1) {
  if (p1)
    A(0);
}

B::B() : A(1) {}

C::C(int) : A(1) {}

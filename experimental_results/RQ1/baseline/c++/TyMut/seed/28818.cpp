
struct A {};
struct B : virtual A {};
struct C : B {
  C() : B(B()) {}
};
int main() { C c; return 0; }


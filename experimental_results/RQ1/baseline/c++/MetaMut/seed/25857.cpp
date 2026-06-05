
struct A { int a; };
struct B : virtual A { int b; };
int B::*memptr_b = &B::a;  // error

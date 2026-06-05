

struct A { virtual void foo () {} };
struct B : virtual A {};
struct C : virtual A {}; 
struct D : C, B, C {};

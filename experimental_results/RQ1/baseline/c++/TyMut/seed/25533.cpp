
struct A { int a; };
struct B { int b; };
struct C { int c; };
struct D : virtual A { int d; };
struct E : virtual B {
  virtual void foo();  // Issues a vfptr.
  int e;
};
struct F: virtual C, D, E { int f; };
F f;


struct C0 { int x; };
struct C1 : C0 { virtual void f(); int y; };
// Converting C0* to C1* is non-trivial

struct A {
  virtual C0 *foo();  // in vftable slot #0.
  virtual C0 *bar();  // in vftable slot #1.
};

struct B : virtual A {
  virtual C1 *foo(); // appended to the A subobject's vftable in slot #2.
};

struct C : virtual A {
  virtual C1 *bar(); // appended to the A subobject's vftable in slot #2.
};

struct D : B, C {};
// It's ambiguous what's in the slot #2 of the A subobject's vftable.

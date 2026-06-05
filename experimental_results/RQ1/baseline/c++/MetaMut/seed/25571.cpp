
  struct A { struct { int n; }; };
  template<int A::*> struct X {};

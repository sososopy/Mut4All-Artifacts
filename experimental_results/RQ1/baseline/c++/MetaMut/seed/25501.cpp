
  struct A {
    virtual void f();  // Expects "(A*)this" in ECX
  };
  struct B : virtual A {
    virtual void f();  // Expects "(char*)(B*)this + 12" in ECX
    virtual ~B();      // Might call f()
  };

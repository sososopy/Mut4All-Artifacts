
struct A {
  virtual void foo (int x) { C e(&f, &x); bar (); }
  virtual bool bar ();
  struct C { C (int *, int *); };
  int f;
};
struct B : A {} d;

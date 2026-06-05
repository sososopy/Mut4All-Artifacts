

struct A {
  explicit A() { }   // remove explicit -> compiles fine
  A(int x) : A() { }
};



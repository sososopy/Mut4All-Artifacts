struct S {
  int f(int);
};

using T = int (S::*)(this int);  // Invalid
T t = &S::f;
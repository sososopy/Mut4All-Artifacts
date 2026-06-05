struct A {
  int a1;
  short a2;
  short a3;
  long a4;
  constexpr A() : a1(), a2(), a3(), a4() { return; }
};

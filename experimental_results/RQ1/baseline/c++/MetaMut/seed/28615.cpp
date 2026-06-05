
struct A {
  union {
    int a;
    int b;
  };
  int& x() { return 0 ? a : b; }
};

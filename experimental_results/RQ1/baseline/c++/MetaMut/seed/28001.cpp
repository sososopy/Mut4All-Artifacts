struct A {
  int *b(const int & = []() -> int { return 0; }(), const int & = []() -> int { return 0; }());
};
int *A::b(const int &, const int &) { b(); return 0; }


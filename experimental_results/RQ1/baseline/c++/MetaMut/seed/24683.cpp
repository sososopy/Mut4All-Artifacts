
  struct T { void operator++(int); };
  void f() { (T())++; }

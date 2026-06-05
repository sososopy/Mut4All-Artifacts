
  typedef int foo;
  struct bar {
    operator int() { return 0; }
    operator foo() { return 0; }
  };

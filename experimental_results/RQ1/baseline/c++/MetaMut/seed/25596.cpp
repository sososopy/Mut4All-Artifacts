

struct A {
  struct B {
    // B() {}  // Uncommenting this line makes it work.
    int y = 1; // Removing initializer makes it work.
  };

  A(const B& opts = B()) {}  // Removing default makes it work.
};


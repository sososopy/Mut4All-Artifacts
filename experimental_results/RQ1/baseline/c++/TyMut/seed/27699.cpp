struct X {
  X() = delete;
  X(int) { }
};

X x[1] { 1 };  // OK
X y[1] { 0 };


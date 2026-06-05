
struct X1 {
  struct X2 {
    int L = ([] (int i) { return i; })(2);
  };
};


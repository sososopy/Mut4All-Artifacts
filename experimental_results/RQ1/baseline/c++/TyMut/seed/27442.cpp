
struct P {
  int x = 0;
};

template<class T>
struct S {
  S() { new P[2][2]; }
};

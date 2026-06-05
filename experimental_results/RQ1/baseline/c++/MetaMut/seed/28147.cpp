
struct S {
  int a;
  int b;
};

template<S s = {1, 2}> struct X {};
X x; // ok, X<{1, 2}>


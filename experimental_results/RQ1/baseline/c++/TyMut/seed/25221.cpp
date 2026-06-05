
int gvar;

template<typename F> void tfunc2(int, F) {}

template<int I> void tfunc() { // ICE is caused when, in function templates,
  const int a = gvar; // a const variable initialized by a non-const value
  tfunc2(a, [&]() {
    tfunc2(a, [&]() {
      (void) a; // is used in nested lambda expressions.
    });
  });
}

int main() {
  tfunc<1>();
  return 0;
}



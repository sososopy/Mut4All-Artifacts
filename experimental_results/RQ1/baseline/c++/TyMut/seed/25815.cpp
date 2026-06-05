
struct S {
  int foo(){ return 0; }
};

template<class... Args>
void evaluate(Args...){}

template <class... Args>
void bar(Args... args) {
  evaluate(args.foo()...); // OK
  auto lmb = [=](){ evaluate(args.foo()...); }; // Error
  lmb();
}

int main() {
  S s{};
  bar(s);
}

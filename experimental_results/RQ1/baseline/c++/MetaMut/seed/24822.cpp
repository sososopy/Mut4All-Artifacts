class X {
public:
    template <typename Fn>
    X(Fn&& F);

    void m();
};

template <class T>
struct Y {
  void operator()() const {
      T a;
      X x = X([&]() {
        undeclared_function(&a);
      });
      x.m();
  }
};

void fun() {
  Y<char>()();
}
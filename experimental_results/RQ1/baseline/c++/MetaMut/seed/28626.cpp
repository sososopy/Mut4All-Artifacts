
template <class>
struct Qux {
  struct A { } a_;

  void AsyncOp() {
    [this](auto) {
      decltype(a_) x;
      struct Grault : decltype(a_) {};
      Grault ptr;
    }(0);
  }
};

void corge() {
  Qux<int> qux;
  qux.AsyncOp();
}

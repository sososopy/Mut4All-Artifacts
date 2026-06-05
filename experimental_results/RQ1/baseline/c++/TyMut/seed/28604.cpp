
void f(auto i) {
  ([]<class... T>(T...) {
    return (..., [] {
      if constexpr (T{0})
        ;
    }());
  }(i));
}
void g() { f(0); }

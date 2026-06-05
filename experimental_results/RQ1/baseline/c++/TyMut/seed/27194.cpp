template <typename T>
struct S {
  using T ::operator<;
  void operator==(T x) { [x] { 0 < x; }; }
};

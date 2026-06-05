
struct S {
  virtual constexpr int foo () const { return 42; }
};

constexpr S s;
constexpr auto a = &S::foo;
constexpr auto b = (s.*a) ();
constexpr auto c = (s.*&S::foo) ();


struct E { int d[10]; };

struct S {
  constexpr int operator()(char) { return 42; }
};

template <typename> struct X {
  constexpr static E foo(S s) { return {{s(1)}}; }
};

S s;
static_assert((X<S>::foo(s), 1), "");

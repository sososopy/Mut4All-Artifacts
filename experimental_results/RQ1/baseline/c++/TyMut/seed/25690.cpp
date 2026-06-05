
struct X {
  int x : 1;
  constexpr static int f(int x) {
    return X{x}.x;
  }
};
static_assert(X::f(3) == -1, "3 should truncate to -1");

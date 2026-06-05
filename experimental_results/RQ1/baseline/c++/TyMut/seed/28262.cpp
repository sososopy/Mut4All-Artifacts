
struct I {
  int* i = new int{1};
  constexpr ~I() { delete i; }
};
static_assert([] { return I{}.i; }());
static_assert(I{}.i);


struct Jam
{
  // constexpr  // n.b.
  int ft() { return 42; }

  constexpr
  Jam() { ft(); }
};

constexpr bool test()
{
  Jam j;
  return true;
}

static_assert(test(), "");


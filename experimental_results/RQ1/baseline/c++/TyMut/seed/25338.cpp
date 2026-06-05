

struct T {
  static constexpr auto Create() { return 0; }
};

void test()
{
  auto v0 = ([](){ return T(); })(); // not a constexpr
  static_assert(0 == v0.Create(), ""); // pass

  constexpr auto v1 = ([](){ return T(); })().Create(); // error
}


